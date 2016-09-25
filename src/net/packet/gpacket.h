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
  typedef enum {
    Eof = -2,    // read
    Fail = -1,   // read write
    TimeOut = 0, // read
    Ok = 1,      // read write
  } Result;

  typedef enum {
    Eth,   // DLT_EN10MB (1)
    Dot11, // DLT_IEEE802_11_RADIO (127)
    Ipv4,  // DLT_IPV4 (228)
    Null,  // DLT_NULL (0)
  } DataLinkType;

  friend struct GParser;

public:
  GPacket();
  GPacket(GCapture* capture);
  ~GPacket() override;

  void clear();

  GCapture* capture_;

  pcap_pkthdr pkthdr_;
  u_char* buf_;

  u_char* parseBuf_; // for parsing
  size_t parseLen_;  // for parsing

protected:
  GPdus pdus_;

public:
  GPdu* first() { return pdus_.first(); }
  GPdu* next() { return pdus_.next(); }
  GPdu* prev() { return pdus_.prev(); }

  template <typename T>
  T* findFirst() { return pdus_.findFirst<T>(); }

  template <typename T>
  T* findNext() { return pdus_.findNext<T>(); }

  template <typename T>
  T* findPrev() { return pdus_.findPrev<T>(); }
};
