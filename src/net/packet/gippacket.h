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

#include "net/packet/gpacket.h"
#include "net/pdu/giphdr.h"
#include "net/pdu/gtcphdr.h"
#include "net/pdu/gudphdr.h"

// ----------------------------------------------------------------------------
// GIpPacket
// ----------------------------------------------------------------------------
struct GIpPacket : GPacket {
  GIpPacket() : GPacket() {}
  GIpPacket(GCapture* capture) : GPacket(capture) {}

public:
  void clear() override {
    GPacket::clear();

    ipHdr_ = nullptr;
    tcpHdr_ = nullptr;
    udpHdr_ = nullptr;
  }

public:
  GIpHdr* ipHdr_{nullptr};
  // GIp6Hdr* ip6Hdr; // gilgil temp 2019.05.13
  GTcpHdr* tcpHdr_{nullptr};
  GUdpHdr* udpHdr_{nullptr};
};
