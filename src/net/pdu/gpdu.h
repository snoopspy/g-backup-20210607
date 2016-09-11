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
#include "net/packet/gpacket.h"
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
  } Flag;

  virtual Flag flag() { return GNone; }
  virtual size_t size() { return 0; }
};

// ----------------------------------------------------------------------------
// GPdus
// ----------------------------------------------------------------------------
struct GPdus : QVector<GPdu*> {
  GPdus::iterator findIterator(GPdu::Flag flag);
};
