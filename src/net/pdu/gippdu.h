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
// IP_HDR
// ----------------------------------------------------------------------------
typedef libnet_ipv4_hdr IP_HDR;

// ----------------------------------------------------------------------------
// GIpPdu
// ----------------------------------------------------------------------------
struct GIpPdu : GPdu {
  static const GPduType staticType = GPduType::Ip;
  GPduType pduType() override { return staticType; }
  size_t size() override;

  GIpPdu(u_char* buf);

  IP_HDR* ipHdr_;
};

// ----------------------------------------------------------------------------
// GIpParser
// ----------------------------------------------------------------------------
struct GIpParser : GParser {
  Q_OBJECT

public:
  Q_INVOKABLE GIpParser(QObject* parent = nullptr) : GParser(parent) {}

protected:
  bool isMatch(GPdu* prev, GPacket* packet) override;
  GPdu* doParse(GPacket* packet) override;
};
