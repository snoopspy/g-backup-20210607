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

#include <pcap.h>
#include <QVector>
#include "net/pdu/gpdu.h"

// ----------------------------------------------------------------------------
// GPacket
// ----------------------------------------------------------------------------
struct GCapture;
struct GPacket : QObject {
  Q_OBJECT
  Q_ENUMS(DataLinkType)

public:
  // --------------------------------------------------------------------------
  // Result
  // --------------------------------------------------------------------------
  typedef enum {
    Eof = -2,    // read
    Fail = -1,   // read write
    TimeOut = 0, // read
    Ok = 1,      // read write
  } Result;

  // --------------------------------------------------------------------------
  // DataLinkType
  // --------------------------------------------------------------------------
  typedef enum {
    Eth,   // DLT_EN10MB (1)
    Dot11, // DLT_IEEE802_11_RADIO (127)
    Ipv4,  // DLT_IPV4 (228)
    Null,  // DLT_NULL (0)
  } DataLinkType;
  static int dataLinkTypeToInt(DataLinkType dataLinkType);
  static DataLinkType intToDataLinkType(int dataLink);

  // --------------------------------------------------------------------------
  // Buf
  // --------------------------------------------------------------------------
  struct Buf {
    u_char* data_;
    size_t size_;
  };
  // --------------------------------------------------------------------------

public:
  GPacket();
  GPacket(GCapture* capture);
  ~GPacket() override;
  void clear();

  GCapture* capture_;

  struct timeval ts_;
  Buf buf_;
  Buf parse_;

  struct {
    bool block_;
  } control;

  GPdus pdus_;
};
