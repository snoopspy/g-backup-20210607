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

// ----------------------------------------------------------------------------
// GEthHdr
// ----------------------------------------------------------------------------
#pragma pack(push, 1)
struct G_EXPORT GEthHdr final { // libnet_ethernet_hdr // gilgil temp 2019.05.13
  GMac     dmac_;
  GMac     smac_;
  uint16_t type_;

  GMac     dmac() { return dmac_; }
  GMac     smac() { return smac_; }
  uint16_t type() { return ntohs(type_); }

  typedef enum {
    IP4 = 0x0800,
    ARP = 0x0806,
    IP6 = 0x86DD
  } Type;
};
typedef GEthHdr *PEthHdr;
#pragma pack(pop)
