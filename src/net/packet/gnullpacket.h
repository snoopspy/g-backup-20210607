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
// GNullPacket
// ----------------------------------------------------------------------------
struct G_EXPORT GNullPacket : GPacket {
  GNullPacket(QObject* parent = nullptr) : GPacket(parent) {
    dataLinkType_ = GPacket::Null;
  }

public:
  void clear() override {
    GPacket::clear();
  }
  void parse() override;
};
typedef GNullPacket *PNullPacket;
