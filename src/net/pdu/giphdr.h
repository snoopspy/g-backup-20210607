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
#include "net/gip.h"

// ----------------------------------------------------------------------------
// IP_HDR
// ----------------------------------------------------------------------------
#pragma pack(push, 1)
struct IP_HDR { // libnet_ipv4_hdr
  uint8_t  ip_hl:4;
  uint8_t  ip_v:4;
  uint8_t  ip_tos;
  uint16_t ip_len;
  uint16_t ip_id;
  uint16_t ip_off;
  uint8_t  ip_ttl;
  uint8_t  ip_p;
  uint16_t ip_sum;
  GIp      ip_sip;
  GIp      ip_dip;

  uint8_t  v()   { return ip_v;          }
  uint8_t  hl()  { return ip_hl;         }
  uint8_t  tos() { return ip_tos;        }
  uint16_t len() { return ntohs(ip_len); }
  uint16_t id()  { return ntohs(ip_id);  }
  uint16_t off() { return ntohs(ip_off); }
  uint8_t  ttl() { return ip_ttl;        }
  uint8_t  p()   { return ip_p;          }
  uint16_t sum() { return ntohs(ip_sum); }
  GIp      sip() { return ntohl(ip_sip); }
  GIp      dip() { return ntohl(ip_dip); }
};
#pragma pack(pop)

// ----------------------------------------------------------------------------
// GIpHdr
// ----------------------------------------------------------------------------
struct GIpHdr : GPdu {
  static const GPdu::Type staticType = GPdu::Type::Ip;
  GPdu::Type pduType() override { return staticType; }
  size_t size() override;

  GIpHdr(u_char* buf);

  uint8_t  v()   { return ip_hdr_->v();   }
  uint8_t  hl()  { return ip_hdr_->hl();  }
  uint8_t  tos() { return ip_hdr_->tos(); }
  uint16_t len() { return ip_hdr_->len(); }
  uint16_t id()  { return ip_hdr_->id();  }
  uint16_t off() { return ip_hdr_->off(); }
  uint8_t  ttl() { return ip_hdr_->ttl(); }
  uint8_t  p()   { return ip_hdr_->p();   }
  uint16_t sum() { return ip_hdr_->sum(); }
  GIp      sip() { return ip_hdr_->sip(); }
  GIp      dip() { return ip_hdr_->dip(); }

protected:
  IP_HDR* ip_hdr_;
};

// ----------------------------------------------------------------------------
// GIpParser
// ----------------------------------------------------------------------------
#include "net/parser/gparser.h"
struct GIpParser : GParser {
  Q_OBJECT

public:
  Q_INVOKABLE GIpParser(QObject* parent = nullptr) : GParser(parent) {}

protected:
  GPdu* doParse(GPacket* packet, GPdu* prev) override;
};
