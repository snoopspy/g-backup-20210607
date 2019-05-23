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

#include "gippacket.h"
#include "net/pdu/gethhdr.h"
#include "net/pdu/garphdr.h"

// ----------------------------------------------------------------------------
// GEthPacket
// ----------------------------------------------------------------------------
struct G_EXPORT GEthPacket : GIpPacket {
  GEthPacket(QObject* parent = nullptr) : GIpPacket(parent) {
    dataLinkType_ = GPacket::Eth;
  }

public:
  void clear() override {
    GIpPacket::clear();
    ethHdr_ = nullptr;
    arpHdr_ = nullptr;
  }

public:
  GEthHdr* ethHdr_{nullptr};
  GArpHdr* arpHdr_{nullptr};
};
typedef GEthPacket *PEthPacket;
