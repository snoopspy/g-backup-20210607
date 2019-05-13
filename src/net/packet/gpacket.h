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
// #include <QVector> // gilgil temp 2019.05.13

// #include "net/pdu/gpdu.h" // gilgil temp 2019.05.12

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

  // --------------------------------------------------------------------------
  // Buf
  // --------------------------------------------------------------------------
  struct Buf {
    u_char* data_;
    size_t size_;
  };
  // --------------------------------------------------------------------------

public:
  GPacket() : QObject(nullptr), capture_(nullptr) {
    clear();
  }

  GPacket(GCapture* capture) : QObject(nullptr) , capture_(capture) {
    clear();
  }

  ~GPacket() override {}

  void clear() {
    bzero(&ts_, sizeof(ts_));
    bzero(&buf_, sizeof(buf_));
    // bzero(&parse_, sizeof(parse_)); // gilgil temp 2019.05.12
    control.block_ = false;
    // pdus_.clear(); // gilgil temp 2019.05.12
  }

  GCapture* capture_;
  struct timeval ts_;
  Buf buf_;
  // Buf parse_; // gilgil temp 2019.05.12

  struct {
    bool block_;
  } control;

  // GPdus pdus_; // gilgil temp 2019.05.12
};
