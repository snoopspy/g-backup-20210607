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

#include "net/capture/gpcapdevice.h"
#include "net/packet/gpacket.h"
#include "net/gnetintf.h"
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
  bool allResolved();
  void deleteUnresolved();
  bool waitAll(GPcapDevice* device, unsigned long timeout = G::Timeout);
  GMac waitOne(GIp ip, GPcapDevice* device, unsigned long timeout = G::Timeout);

protected:
  bool sendQueries(GPcapDevice* device, GNetIntf* intf);

public:
  static GAtm& instance();

protected:
  // --------------------------------------------------------------------------
  // SendThread
  // --------------------------------------------------------------------------
  struct SendThread : GThread {
    SendThread(GAtm* resolve, GPcapDevice* device, GNetIntf* intf, unsigned long timeout);
    GAtm* atm_;
    GPcapDevice* device_;
    GNetIntf* intf_;
    unsigned long timeout_;
    GWaitEvent we_;
  protected:
    void run() override;
  };
};

