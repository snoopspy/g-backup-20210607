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
struct ETH_HDR { // libnet_ethernet_hdr
  GMac     ether_dhost;
  GMac     ether_shost;
  uint16_t ether_type;

  GMac dmac()     { return ether_dhost; }
  GMac smac()     { return ether_shost; }
  uint16_t type() { return ntohs(ether_type); }
};
#pragma pack(pop)

// ----------------------------------------------------------------------------
// GEthHdr
// ----------------------------------------------------------------------------
struct GEthHdr : GPdu {
  static const GPdu::Type staticType = GPdu::Type::Eth;
  GPdu::Type pduType() override { return staticType; }
  GPdu::Id nextPduId() override { return (GPdu::Id)type(); }
  size_t size() override;

  GEthHdr(u_char* buf);

  GMac     dmac() { return eth_hdr_->dmac(); }
  GMac     smac() { return eth_hdr_->smac(); }
  uint16_t type() { return eth_hdr_->type(); }

protected:
  ETH_HDR* eth_hdr_;
};

// ----------------------------------------------------------------------------
// GEthParser
// ----------------------------------------------------------------------------
#include "net/parser/gparser.h"
struct GEthParser : GParser {
  Q_OBJECT

public:
  Q_INVOKABLE GEthParser(QObject* parent = nullptr) : GParser(parent) {}

protected:
  GPdu* doParse(GPacket* packet) override;
};
