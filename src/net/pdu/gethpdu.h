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

#include "gpdu.h"
#include "net/gmac.h"

// ----------------------------------------------------------------------------
// ETH_HDR
// ----------------------------------------------------------------------------
struct ETH_HDR {
  GMac dst;
  GMac src;
  uint16_t type;
};

// ----------------------------------------------------------------------------
// GEthPdu
// ----------------------------------------------------------------------------
struct GEthPdu : GPdu {
  GPdu::Flag flag() override { return GPdu::GEthPdu; }
  size_t size() { return len_; }

  GEthPdu(u_char* buf, size_t len);

  ETH_HDR* ethHdr_;
  size_t len_;
};
