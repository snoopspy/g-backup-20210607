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
// GEthHdr
// ----------------------------------------------------------------------------
#pragma pack(push, 1)
struct GEthHdr final { // libnet_ethernet_hdr // gilgil temp 2019.05.13
  GMac     dmac_;
  GMac     smac_;
  uint16_t type_;

  GMac     dmac() { return dmac_; }
  GMac     smac() { return smac_; }
  uint16_t type() { return ntohs(type_); }

  typedef enum {
    Ip  = 0x0800,
    Arp = 0x0806,
    Ip6 = 0x86DD
  } Type;
};
#pragma pack(pop)
