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
// GIpHdr
// ----------------------------------------------------------------------------
#pragma pack(push, 1)
struct G_EXPORT GIp6Hdr final {
  uint8_t  v_pr_;     // version, priority
  uint8_t  flags_[3]; // traffic class, flow label
  uint16_t len_;      // total length
  uint8_t  nh_;       // next header
  uint8_t  hl_;       // hop limit
  GIp6     sip_;      // source ip
  GIp6     dip_;      // destination ip

  uint8_t  v()     { return (v_pr_ & 0xF0) >> 4; }
  uint8_t  pr()    { return v_pr_ & 0x0F; }
  uint8_t* flags() { return flags_; }
  uint8_t  nh()    { return nh_; }
  uint8_t  hl()    { return hl_; }
  GIp6     sip()   { return sip_; }
  GIp6     dip()   { return dip_; }
};
#pragma pack(pop)
