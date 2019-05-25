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

#include "net/packet/gethpacket.h"
#include "net/packet/gippacket.h"
#include "net/packet/gdot11packet.h"
#include "net/packet/gnullpacket.h"

// ----------------------------------------------------------------------------
// GPacketCast
// ----------------------------------------------------------------------------
struct GPacketCast {
  static GEthPacket* toEth(GPacket* packet) {
    switch (packet->dataLinkType_) {
      case GPacket::Eth:
        Q_ASSERT(dynamic_cast<GEthPacket*>(packet) != nullptr);
        return PEthPacket(packet);
      case GPacket::Ip:
        Q_ASSERT(dynamic_cast<GEthPacket*>(packet) != nullptr);
        return PEthPacket(packet);
      case GPacket::Dot11:
        return nullptr;
      case GPacket::Null:
        return nullptr;
    }
  }

  static GIpPacket* toIp(GPacket* packet) {
    switch (packet->dataLinkType_) {
      case GPacket::Eth:
        Q_ASSERT(dynamic_cast<GIpPacket*>(packet) != nullptr);
        return PIpPacket(packet);
      case GPacket::Ip:
        Q_ASSERT(dynamic_cast<GIpPacket*>(packet) != nullptr);
        return PIpPacket(packet);
      case GPacket::Dot11:
        return nullptr;
      case GPacket::Null:
        return nullptr;
    }
  }

  static GDot11Packet* toDot11(GPacket* packet) {
    switch (packet->dataLinkType_) {
      case GPacket::Eth:
        return nullptr;
      case GPacket::Ip:
        return nullptr;
      case GPacket::Dot11:
        Q_ASSERT(dynamic_cast<GDot11Packet*>(packet) != nullptr);
        return PDot11Packet(packet);
      case GPacket::Null: return nullptr;
    }
  }

  static GNullPacket* toNull(GPacket* packet) {
    Q_ASSERT(dynamic_cast<GNullPacket*>(packet) != nullptr);
    return PNullPacket(packet);
  }
};
