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
// GDot11Packet
// ----------------------------------------------------------------------------
struct G_EXPORT GDot11Packet : GPacket {
  GDot11Packet(QObject* parent = nullptr) : GPacket(parent) {
    dataLinkType_ = GPacket::Dot11;
  }

public:
  void clear() override {
    GPacket::clear();
  }
  void parse() override;
};
typedef GDot11Packet *PDot11Packet;
