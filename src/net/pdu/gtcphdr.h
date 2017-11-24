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
// TCP_HDR
// ----------------------------------------------------------------------------
#pragma pack(push, 1)
struct TCP_HDR { // libnet_tcp_hdr
  uint16_t th_sport;
  uint16_t th_dport;
  uint32_t th_seq;
  uint32_t th_ack;
  uint8_t  th_x2:4;
  uint8_t  th_off:4;
  uint8_t  th_flags;
  uint16_t th_win;
  uint16_t th_sum;
  uint16_t th_urp;

  uint16_t sport() { return ntohs(th_sport); }
  uint16_t dport() { return ntohs(th_dport); }
  uint32_t seq()   { return ntohl(th_seq);   }
  uint32_t ack()   { return ntohl(th_ack);   }
  uint8_t  x2()    { return th_x2;           }
  uint8_t  off()   { return th_off;          }
  uint8_t  flags() { return th_flags;        }
  uint16_t win()   { return ntohs(th_win);   }
  uint16_t sum()   { return ntohs(th_sum);   }
  uint16_t urp()   { return ntohs(th_urp);   }
};
#pragma pack(pop)

// ----------------------------------------------------------------------------
// GTcpHdr
// ----------------------------------------------------------------------------
struct GTcpHdr : GPdu {
  static const GPdu::Type staticType = GPdu::Type::Tcp;
  GPdu::Type pduType() override { return staticType; }
  GPdu::Id nextPduId() override { return NULL_PDU_ID; }
  size_t size() override;

  GTcpHdr(u_char* buf);

  uint16_t sport() { return tcp_hdr_->sport(); }
  uint16_t dport() { return tcp_hdr_->dport(); }
  uint32_t seq()   { return tcp_hdr_->seq();   }
  uint32_t ack()   { return tcp_hdr_->ack();   }
  uint8_t  x2()    { return tcp_hdr_->x2();    }
  uint8_t  off()   { return tcp_hdr_->off();   }
  uint8_t  flags() { return tcp_hdr_->flags(); }
  uint16_t win()   { return tcp_hdr_->win();   }
  uint16_t sum()   { return tcp_hdr_->sum();   }
  uint16_t urp()   { return tcp_hdr_->urp();   }

protected:
  TCP_HDR* tcp_hdr_;
};

// ----------------------------------------------------------------------------
// GTcpParser
// ----------------------------------------------------------------------------
#include "net/parser/gparser.h"
struct GTcpParser : GParser {
  Q_OBJECT

public:
  Q_INVOKABLE GTcpParser(QObject* parent = nullptr) : GParser(parent) {}

protected:
  GPdu* doParse(GPacket* packet) override;
};
