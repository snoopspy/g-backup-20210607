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
#include "net/pdu/gip6hdr.h"
#include "net/pdu/gtcphdr.h"
#include "net/pdu/gudphdr.h"
#include "net/pdu/gicmphdr.h"

// ----------------------------------------------------------------------------
// GIpPacket
// ----------------------------------------------------------------------------
struct G_EXPORT GIpPacket : GPacket {
  GIpPacket(QObject* parent = nullptr) : GPacket(parent) {
    dataLinkType_ = GPacket::Ip;
  }

public:
  void clear() override {
    GPacket::clear();
    ipHdr_   = nullptr;
    ip6Hdr_  = nullptr;
    tcpHdr_  = nullptr;
    udpHdr_  = nullptr;
    icmpHdr_ = nullptr;
    tcpData_.clear();
    udpData_.clear();
  }
  void parse() override;

public:
  GIpHdr*   ipHdr_{nullptr};
  GIp6Hdr*  ip6Hdr_{nullptr};

  GTcpHdr*  tcpHdr_{nullptr};
  GUdpHdr*  udpHdr_{nullptr};
  GIcmpHdr* icmpHdr_{nullptr};

  GBuf tcpData_;
  GBuf udpData_;
};
typedef GIpPacket *PIpPacket;
