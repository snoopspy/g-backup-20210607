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
#include "net/pdu/gdnshdr.h"

// --------------------------------------------------------------------------
// GDns
// --------------------------------------------------------------------------
struct G_EXPORT GDns {
  // ------------------------------------------------------------------------
  // Question
  // ------------------------------------------------------------------------
  struct G_EXPORT Question {
    QString name_;
    uint16_t type_;
    uint16_t class_;

    QByteArray encode();
    bool decode(u_char* udpData, size_t dataLen, size_t* offset);
  };

  struct G_EXPORT Questions : QList<Question> {
    QByteArray encode();
    bool decode(u_char* udpData, size_t dataLen, int count, size_t* offset);
  };

  // ------------------------------------------------------------------------
  // ResourceRecord
  // ------------------------------------------------------------------------
  struct G_EXPORT ResourceRecord {
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
  struct G_EXPORT ResourceRecords : QList<ResourceRecord> {
    QByteArray encode();
    bool decode(u_char* udpData, size_t dataLen, int count, size_t* offset);
  };
  // ------------------------------------------------------------------------

  GDnsHdr dnsHdr_;
  Questions questions_;
  ResourceRecords answers_;
  ResourceRecords authorities_;
  ResourceRecords additionals_;

  QByteArray encode();
  bool decode(u_char* udpData, size_t dataLen, size_t* offset);

  static QByteArray encodeName(QString name);
  static QString decodeName(u_char* udpData, size_t dataLen, size_t* offset);
};

