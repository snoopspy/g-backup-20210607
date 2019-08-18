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
	Q_PROPERTY(bool enabled MEMBER enabled_)
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
	bool enabled_{true};
	GIp senderIp_{uint32_t(0)};
	GMac senderMac_{GMac::cleanMac()};
	GIp targetIp_{uint32_t(0)};
	GMac targetMac_{GMac::cleanMac()};
};
typedef GArpSpoofSession *PArpSpoofSession;

// ----------------------------------------------------------------------------
// GArpSpoof
// ----------------------------------------------------------------------------
struct G_EXPORT GArpSpoof : GPcapDevice {
	Q_OBJECT
	Q_PROPERTY(QString virtualMac READ getVirtualMac WRITE setVirtualMac)
	Q_PROPERTY(ulong infectInterval MEMBER infectInterval_)
	Q_PROPERTY(GObjRefArrayPtr sessions READ getSessions)

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
	GPacket::Result relay(GPacket* packet) override;

	PathType pathType() override { return InPath; }

protected:
	void run() override;

public:
	GMac virtualMac_{GMac::cleanMac()};
	GDuration infectInterval_{1000};
	GObjRefArray<GArpSpoofSession> propSessions_; // for property

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
	typedef QMap<GFlow::IpFlowKey, Session> SessionMap;
	SessionMap sessionMap_; // for relay

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
