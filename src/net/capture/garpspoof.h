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
	Q_PROPERTY(QString virtualMac READ getVirtualMac WRITE setVirtualMac)
	Q_PROPERTY(ulong infectInterval MEMBER infectInterval_)

	GObjRefArrayPtr getSessions() { return &propSessions_; }
	QString getVirtualMac() { return QString(virtualMac_); }
	void setVirtualMac(QString value) { virtualMac_ = value; }

public:
  Q_INVOKABLE GArpSpoof(QObject* parent = nullptr);
  ~GArpSpoof() override;

protected:
  bool doOpen() override;
  bool doClose() override;

public:
	GObjRefArray<GArpSpoofSession> propSessions_; // for property
	GMac virtualMac_{GMac::cleanMac()};
	GDuration infectInterval_{1000};

protected:
	struct Session {
		GIp senderIp_;
		GMac senderMac_;
		GIp targetIp_;
		GMac targetMac_;

		Session() {}
		Session(GIp senderIp, GMac senderMac, GIp targetIp, GMac targetMac) :
			senderIp_(senderIp), senderMac_(senderMac), targetIp_(targetIp), targetMac_(targetMac) {}
	};
	QList<Session> sessionList_; // for arp infect and recover
	QMap<GFlow::IpFlowKey, Session> sessionMap_; // for relay

	GNetIntf intf_;
	GAtm atm_;
	GMac attackMac_{GMac::cleanMac()};

	struct InfectThread : GThread {
		InfectThread(GArpSpoof* arpSpoof) : GThread(arpSpoof), arpSpoof_(arpSpoof) {}
		void run() override;
		GWaitEvent we_;
		GArpSpoof* arpSpoof_;
	} infectThread_{this};

	bool sendArpInfectAll();
	bool sendArpInfect(Session* session);
	bool sendARPReciverAll();
	bool sendArpRecover(Session* session);
	bool sendArp(Session* session, bool infect);
};
