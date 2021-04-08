// ----------------------------------------------------------------------------
//
// G Library
//
// http://gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

#include "net/capture/gpcapdevice.h"
#include "net/packet/gpacket.h"
#include "net/gnetinfo.h"
#include "base/sys/gwaitevent.h"

// ----------------------------------------------------------------------------
// GEthArpHdr
// ----------------------------------------------------------------------------
#pragma pack(push, 1)
struct GEthArpHdr {
	GEthHdr ethHdr_;
	GArpHdr arpHdr_;
};
#pragma pack(pop)

// ----------------------------------------------------------------------------
// GAtm(Arp Table Manager)
// ----------------------------------------------------------------------------
typedef QMap<GIp, GMac> GAtmMap;
struct G_EXPORT GAtm : GAtmMap {
	friend struct GAtmMgr;

protected:
	~GAtm() {} // can not create in other class

public:
	bool allResolved();
	void deleteUnresolved();
	bool wait(GPcapDevice* device, GDuration timeout = G::Timeout);

protected:
	bool sendQueries(GPcapDevice* device, GInterface* intf);

public:
	static GAtm& instance(QString intfName);

protected:
	// --------------------------------------------------------------------------
	// SendThread
	// --------------------------------------------------------------------------
	struct SendThread : QThread {
		SendThread(GAtm* resolve, GPcapDevice* device, GInterface* intf, GDuration timeout);
		GAtm* atm_;
		GPcapDevice* device_;
		GInterface* intf_;
		GDuration timeout_;
		GWaitEvent we_;
	protected:
		void run() override;
	};
};

