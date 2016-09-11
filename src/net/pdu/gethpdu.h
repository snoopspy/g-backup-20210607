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
  size_t size() { return ethHdr_ == nullptr ? 0 : ETHER_ADDR_LEN; }

  GEthPdu(u_char* buf, size_t len);

  ETH_HDR* ethHdr_;
};

// ----------------------------------------------------------------------------
// GEthParser
// ----------------------------------------------------------------------------
struct GEthParser : GParser {
  Q_OBJECT

public:
  Q_INVOKABLE GEthParser(QObject* parent = nullptr) : GParser(parent) {}

protected:
  bool isMatch(GPdu* prev, GPacket* packet) override;
  GPdu* doParse(GPacket* packet) override;
};
// Q_DECLARE_METATYPE(GEthParser) // gilgil temp 2016.09.10
