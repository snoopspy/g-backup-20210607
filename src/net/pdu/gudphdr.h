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
// UDP_HDR
// ----------------------------------------------------------------------------
#pragma pack(push, 1)
struct UDP_HDR { // libnet_tcp_hdr
  uint16_t uh_sport;
  uint16_t uh_dport;
  uint16_t uh_len;
  uint16_t uh_sum;

  uint16_t sport() { return ntohs(uh_sport); }
  uint16_t dport() { return ntohs(uh_dport); }
  uint16_t len()   { return ntohs(uh_len);   }
  uint16_t sum()   { return ntohs(uh_sum);   }
};
#pragma pack(pop)

// ----------------------------------------------------------------------------
// GUdpHdr
// ----------------------------------------------------------------------------
struct GUdpHdr : GPdu {
  static const GPdu::Type staticType = GPdu::Type::Udp;
  GPdu::Type pduType() override { return staticType; }
  GPdu::Id pduId() override { return NULL_PDU_ID; }
  size_t size() override;

  GUdpHdr(u_char* buf);

  uint16_t sport() { return udp_hdr_->sport(); }
  uint16_t dport() { return udp_hdr_->dport(); }
  uint16_t len()   { return udp_hdr_->len();   }
  uint16_t sum()   { return udp_hdr_->sum();   }

protected:
  UDP_HDR* udp_hdr_;
};

// ----------------------------------------------------------------------------
// GUdpParser
// ----------------------------------------------------------------------------
#include "net/parser/gparser.h"
struct GUdpParser : GParser {
  Q_OBJECT

public:
  Q_INVOKABLE GUdpParser(QObject* parent = nullptr) : GParser(parent) {}

protected:
  GPdu* doParse(GPacket* packet) override;
};
