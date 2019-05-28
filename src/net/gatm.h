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
#include "net/pdu/gethhdr.h"
#include "net/pdu/garphdr.h"
#include "base/gwaitevent.h"

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
// GArpResolve
// ----------------------------------------------------------------------------
struct G_EXPORT GArpResolve : GStateObj {
  Q_OBJECT

public:
  Q_INVOKABLE GArpResolve(QObject* parent = nullptr) : GStateObj(parent) {}
  ~GArpResolve() override {}

protected:
  bool doOpen() override { return true; }
  bool doClose() override { return true; }

public:
  // --------------------------------------------------------------------------
  // SendThread
  // --------------------------------------------------------------------------
  struct SendThread : QThread {
    SendThread(GArpResolve* resolve, GPcapDevice* device, GNetIntf* intf, unsigned long timeout);
    GArpResolve* resolve_;
    GPcapDevice* device_;
    GNetIntf* intf_;
    unsigned long timeout_;
    GWaitEvent we_;
  protected:
    void run() override;
  };

  // --------------------------------------------------------------------------
  // ArpTable
  // --------------------------------------------------------------------------
  struct ArpTable : QMap<GIp, GMac> {
    bool ok() {
      for (GMac& mac: *this)
        if (mac.isClean()) return  false;
      return true;
    }
  } arpTable_;
  bool waitResolve(GPcapDevice* device, unsigned long timeout = G::Timeout);

protected:
  bool sendQueries(GPcapDevice* device, GNetIntf* intf);

public slots:
  void resolve(GPacket* packet);

signals:
  void resolved(GPacket* packet);
};

