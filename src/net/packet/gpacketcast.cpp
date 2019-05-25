#include "gpacketcast.h"

// ----------------------------------------------------------------------------
// GPacketCast
// ----------------------------------------------------------------------------
GEthPacket* GPacketCast::toEth(GPacket* packet) {
  GEthPacket* res;
  switch (packet->dataLinkType_) {
    case GPacket::Eth: res = PEthPacket(packet); break;
    case GPacket::Ip: res = PEthPacket(packet); break;
    case GPacket::Dot11: return nullptr;
    case GPacket::Null: return nullptr;
  }
  Q_ASSERT(dynamic_cast<GEthPacket*>(packet) != nullptr);
  return res;
}

GIpPacket* GPacketCast::toIp(GPacket* packet) {
  GIpPacket* res;
  switch (packet->dataLinkType_) {
    case GPacket::Eth: res = PIpPacket(packet); break;
    case GPacket::Ip: res = PIpPacket(packet); break;
    case GPacket::Dot11: return nullptr;
    case GPacket::Null: return nullptr;
  }
  Q_ASSERT(dynamic_cast<GIpPacket*>(packet) != nullptr);
  return res;
}

GDot11Packet* GPacketCast::toDot11(GPacket* packet) {
  GDot11Packet* res;
  switch (packet->dataLinkType_) {
    case GPacket::Eth: return nullptr;
    case GPacket::Ip: return nullptr;
    case GPacket::Dot11: res = PDot11Packet(packet); break;
    case GPacket::Null: return nullptr;
  }
  Q_ASSERT(dynamic_cast<GDot11Packet*>(packet) != nullptr);
  return res;
}

GNullPacket* GPacketCast::toNull(GPacket* packet) {
  Q_ASSERT(dynamic_cast<GNullPacket*>(packet) != nullptr);
  return PNullPacket(packet);
}
