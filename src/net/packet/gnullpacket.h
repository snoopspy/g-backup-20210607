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
struct G_EXPORT GNullPacket : GPacket {
  GNullPacket(QObject* parent = nullptr) : GPacket(parent) {
    dataLinkType_ = GPacket::Null;
  }
};
