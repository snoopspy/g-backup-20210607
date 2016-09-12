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
  static const PduType staticType = GPdu::GEthPdu;
  GPdu::PduType pduType() override { return staticType; }
  size_t size() override;

  GEthPdu(u_char* buf);

  GMac dst() { return ethHdr_->ether_dhost; }
  GMac src() { return ethHdr_->ether_shost; }
  uint16_t type() { return ntohs(ethHdr_->ether_type); }

protected:
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
