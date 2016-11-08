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

#include "net/packet/gpacket.h"

// ----------------------------------------------------------------------------
// DNS_HDR
// ----------------------------------------------------------------------------
#pragma pack(push, 1)
struct DNS_HDR { // libnet_dnsv4_hdr
    uint16_t id_;
    uint16_t flags_;
    uint16_t num_q;
    uint16_t num_answ_rr;
    uint16_t num_auth_rr;
    uint16_t num_addi_rr;

    uint16_t id() { return ntohs(id_); }
    uint16_t flags() { return ntohs(flags_); }
    uint16_t q() { return ntohs(num_q); }
    uint16_t answ() { return ntohs(num_answ_rr); }
    uint16_t auth() { return ntohs(num_auth_rr); }
    uint16_t addr() { return ntohs(num_addi_rr); }
};
#pragma pack(pop)

// --------------------------------------------------------------------------
// GDns
// --------------------------------------------------------------------------
struct GDns {
  // ------------------------------------------------------------------------
  // Question
  // ------------------------------------------------------------------------
  struct Question {
    QString name_;
    uint16_t type_;
    uint16_t class_;

    QByteArray encode();
    bool decode(u_char* udpData, size_t dataLen, size_t* offset);
  };

  struct Questions : QList<Question> {
    QByteArray encode();
    bool decode(u_char* udpData, size_t dataLen, int count, size_t* offset);
  };

  // ------------------------------------------------------------------------
  // ResourceRecord
  // ------------------------------------------------------------------------
  struct ResourceRecord {
    QString name_;
    uint16_t type_;
    uint16_t class_;
    uint32_t ttl_;
    uint16_t dataLength_;
    QByteArray data_;

    QByteArray encode();
    bool decode(u_char* udpData, size_t dataLen, size_t* offset);
  };

  // ------------------------------------------------------------------------
  // ResourceRecords
  // ------------------------------------------------------------------------
  struct ResourceRecords : QList<ResourceRecord> {
    QByteArray encode();
    bool decode(u_char* udpData, size_t dataLen, int count, size_t* offset);
  };
  // ------------------------------------------------------------------------

  DNS_HDR dnsHdr_;
  Questions questions_;
  ResourceRecords answers_;
  ResourceRecords authorities_;
  ResourceRecords additionals_;

  QByteArray encode();
  bool decode(u_char* udpData, size_t dataLen, size_t* offset);

  static QByteArray encodeName(QString name);
  static QString decodeName(u_char* udpData, size_t dataLen, size_t* offset);
};

