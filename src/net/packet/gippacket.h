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
  Q_OBJECT

public:
  GIpHdr* ipHdr_;
  // GIp6Hdr* ip6Hdr; // gilgil temp 2019.05.13
  TCP_HDR* tcpHdr_;
  UDP_HDR* udpHdr_;

public:
  GIpPacket() : GPacket() {}
  GIpPacket(GCapture* capture) : GPacket(capture) {}
};
