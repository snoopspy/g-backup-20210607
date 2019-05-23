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
#include <QObject>
#include "base/gbuf.h"
// #include <QVector> // gilgil temp 2019.05.13

// #include "net/pdu/gpdu.h" // gilgil temp 2019.05.12

// ----------------------------------------------------------------------------
// GPacket
// ----------------------------------------------------------------------------
struct GCapture;
struct G_EXPORT GPacket : QObject {
  Q_OBJECT
  Q_ENUMS(DataLinkType)

public:
  // --------------------------------------------------------------------------
  // Result
  // --------------------------------------------------------------------------
  typedef enum {
    Eof     = -2, // read
    Fail    = -1, // read write
    TimeOut = 0,  // read
    Ok      = 1,  // read write
  } Result;

  // --------------------------------------------------------------------------
  // DataLinkType
  // --------------------------------------------------------------------------
  typedef enum {
    Eth,   // DLT_EN10MB (1)
    Ip,    // DLT_RAW (228)
    Dot11, // DLT_IEEE802_11_RADIO (127)
    Null,  // DLT_NULL (0)
  } DataLinkType;
  static int dataLinkTypeToInt(DataLinkType dataLinkType);
  static DataLinkType intToDataLinkType(int dataLink);

public:
  GPacket(QObject* parent = nullptr) : QObject(parent) { clear(); } // parent may be GCapture

public:
  //
  // info
  //
  DataLinkType dataLinkType_{Null};

  //
  // sniffing
  //
  struct timeval ts_;
  GBuf buf_;

  //
  // control
  //
  struct {
    bool block_{false};
  } ctrl;

public:
  virtual void clear() {
    buf_.clear();
    ctrl.block_ = false;
  }

  // GPdus pdus_; // gilgil temp 2019.05.12
};
