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

#include "gpacket.h"

// ----------------------------------------------------------------------------
// GEthPacket
// ----------------------------------------------------------------------------
struct G_EXPORT GDot11Packet : GPacket {
  GDot11Packet(QObject* parent = nullptr) : GPacket(parent) {
    dataLinkType_ = GPacket::Dot11;
  }
};
