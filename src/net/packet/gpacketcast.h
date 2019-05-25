// ----------------------------------------------------------------------------
//
// G Library
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#include "net/packet/gethpacket.h"
#include "net/packet/gippacket.h"
#include "net/packet/gdot11packet.h"
#include "net/packet/gnullpacket.h"

// ----------------------------------------------------------------------------
// GPacketCast
// ----------------------------------------------------------------------------
namespace GPacketCast {
  GEthPacket* toEth(GPacket* packet);
  GIpPacket* toIp(GPacket* packet);
  GDot11Packet* toDot11(GPacket* packet);
  GNullPacket* toNull(GPacket* packet);
}
