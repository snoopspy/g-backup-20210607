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
#pragma pack(push, 1)
struct ETH_HDR {
  GMac ether_dhost;
  GMac ether_shost;
  uint16_t ether_type;
};
#pragma pack(pop)

// ----------------------------------------------------------------------------
// GEthPdu
// ----------------------------------------------------------------------------
struct GEthPdu : GPdu {
  size_t size() override;
  GPdu::Type type() override { return GPdu::GEthPdu; }

  GEthPdu(u_char* buf);

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
