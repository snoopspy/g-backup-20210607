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
			QString msg = QString("target(%1) can not be my ip").arg(QString(senderIp));
			SET_ERR(GErr::FAIL, msg);
			return false;
		}

		if (senderIp == targetIp) {
			QString msg = QString("sender(%1) and target(%2) can not be same").arg(QString(senderIp), QString(targetIp));
			SET_ERR(GErr::FAIL, msg);
			return false;
		}

		Session session;
		session.senderIp_ = senderIp;
		session.senderMac_ = senderMac;
		session.targetIp_ = targetIp;
		session.targetMac_ = targetMac;

		sessionList_.push_back(session);
		sessionMap_[GFlow::IpFlowKey(session.senderIp_, session.targetIp_)] = session;

		if (atm_.find(senderIp) == atm_.end())
			atm_[session.senderIp_] = session.senderMac_;
		if (atm_.find(targetIp) == atm_.end())
			atm_[session.targetIp_] = session.targetMac_;
	}

	if (!atm_.waitAll(this)) {
		QString msg = "can not find all host ";
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
	return true;
}

bool GArpSpoof::doClose() {
	qDebug() << ""; // gilgil temp 2019.08.16
  return GPcapDevice::doClose();
}
