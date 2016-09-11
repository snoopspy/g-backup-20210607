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

#include <netinet/in.h>
#pragma pack(push, 1)
#include "net/libnet/config.h"
#include "net/libnet/libnet-macros.h"
#include "net/libnet/libnet-headers.h"
#pragma pack(pop)
#include <QVector>
#include "net/parser/gparser.h"

// ----------------------------------------------------------------------------
// GPdu
// ----------------------------------------------------------------------------
struct GPdu {
  typedef enum {
    GEthPdu,
    GIpPdu,
    GIp6Pdu,
    GTcpPdu,
    GTcpOptionPdu,
    GUdpPdu,
    GIcmpPdu,
    GDnsPdu,
    GNone
  } Type;

  virtual ~GPdu() {}

  virtual Type type() { return GNone; }
  virtual size_t size() { return 0; }
};

// ----------------------------------------------------------------------------
// GPdus
// ----------------------------------------------------------------------------
struct GPdus : QVector<GPdu*> {
protected:
  int current_{0};

public:
  void clear();
  void add(GPdu* pdu);

public:
  GPdu* first();
  GPdu* next();
  GPdu* prev();
  GPdu* findFirst(GPdu::Type type);
  GPdu* findNext(GPdu::Type type);
  GPdu* findPrev(GPdu::Type type);
};
