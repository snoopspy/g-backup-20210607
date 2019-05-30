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
// GIpPacket
// ----------------------------------------------------------------------------
struct G_EXPORT GIpPacket : GPacket {
  GIpPacket(QObject* parent = nullptr) : GPacket(parent) {
    dataLinkType_ = GPacket::Ip;
  }

  void parse() override;
};
typedef GIpPacket *PIpPacket;
