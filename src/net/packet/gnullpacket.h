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
struct GNullPacket : GPacket {
  GNullPacket(QObject* parent = nullptr) : GPacket(parent) {
    dataLinkType_ = GPacket::Null;
  }
};
