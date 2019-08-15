// ----------------------------------------------------------------------------
//
// G Library
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

#include "gsyncpcapdevice.h"
#include "net/gatm.h"
#include "net/flow/gflowkey.h"

// ----------------------------------------------------------------------------
// GArpSpoofSession
// ----------------------------------------------------------------------------
struct GArpSpoofSession : GObj {
	Q_OBJECT
	Q_PROPERTY(QString senderIp READ getSenderIp WRITE setSenderIp)
	Q_PROPERTY(QString senderMac READ getSenderMac WRITE setSenderMac)
	Q_PROPERTY(QString targetIp READ getTargetIp WRITE setTargetIp)
	Q_PROPERTY(QString targetMac READ getTargetMac WRITE setTargetMac)

	QString getSenderIp() { return QString(senderIp_); }
	void setSenderIp(QString value) { senderIp_ = value; }
	QString getSenderMac() { return QString(senderMac_); }
	void setSenderMac(QString value) { senderMac_ = value; }
	QString getTargetIp() { return QString(targetIp_); }
	void setTargetIp(QString value) { targetIp_ = value; }
	QString getTargetMac() { return QString(targetMac_); }
	void setTargetMac(QString value) { targetMac_ = value; }

public:
	GIp senderIp_{uint32_t(0)};
	GMac senderMac_{GMac::cleanMac()};
	GIp targetIp_{uint32_t(0)};
	GMac targetMac_{GMac::cleanMac()};
};
typedef GArpSpoofSession *PArpSpoofSession;

// ----------------------------------------------------------------------------
// GArpSpoof
// ----------------------------------------------------------------------------
struct G_EXPORT GArpSpoof : GSyncPcapDevice {
  Q_OBJECT
	Q_PROPERTY(GObjRefArrayPtr sessions READ getSessions)

	GObjRefArrayPtr getSessions() { return &propSessions_; }

public:
  Q_INVOKABLE GArpSpoof(QObject* parent = nullptr);
  ~GArpSpoof() override;

protected:
  bool doOpen() override;
  bool doClose() override;

public:
	GObjRefArray<GArpSpoofSession> propSessions_; // for property

protected:
	GNetIntf intf_;

	struct Session {
		GIp senderIp_;
		GMac senderMac_;
		GIp targetIp_;
		GMac targetMac_;
	};
	QList<Session> sessionList_; // for arp infect and recover
	QMap<GFlow::IpFlowKey, Session> sessionMap_; // for relay
	GAtm atm_;
};
