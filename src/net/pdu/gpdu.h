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
// GPduType
// ----------------------------------------------------------------------------
typedef enum {
  Eth,
  Ip,
  Ip6,

  Tcp,
  Udp,
  Icmp,

  TcpOption,
  TcpData,
  UdpData,

  Dns,
  None
} GPduType;

// ----------------------------------------------------------------------------
// GPdu
// ----------------------------------------------------------------------------
struct GPdu {
  virtual ~GPdu() {}

  static const GPduType staticType = None;
  virtual GPduType pduType() { return staticType; }
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
  GPdu* first() {
    current_ = 0;
    if (size() > 0)
      return at(0);
    return nullptr;
  }

  GPdu* next() {
    Q_ASSERT(current_ >= 0 && current_ < size());
    current_++;
    if (current_ >= 0 && current_ < size())
      return at(current_);
    return nullptr;
  }

  GPdu* prev() {
    Q_ASSERT(current_ >= 0 && current_ < size());
    current_--;
    if (current_ >= 0 && current_ < size())
      return at(current_);
    return nullptr;
  }

  template <typename T>
  T* findFirst(GPduType findType = T::staticType) {
    int count = size();
    for (int i = 0; i < count; i++) {
      GPdu* pdu = at(i);
      if (pdu->pduType() == findType) {
        current_ = i;
        return (T*)pdu;
      }
    }
    return nullptr;
  }

  template <typename T>
  T* findNext(GPduType findType = T::staticType) {
    Q_ASSERT(current_ >= 0 && current_ < size());
    int count = size();
    for (int i = current_ + 1; i < count; i++) {
      GPdu* pdu = at(i);
      if (pdu->pduType() == findType) {
        current_ = i;
        return (T*)pdu;
      }
    }
    return nullptr;
  }

  template <typename T>
  T* findPrev(GPduType findType = T::staticType) {
    Q_ASSERT(current_ >= 0 && current_ < size());
    for (int i = current_ - 1; i >= 0; i--) {
      GPdu* pdu = at(i);
      if (pdu->pduType() == findType) {
        current_ = i;
        return (T*)pdu;
      }
    }
    return nullptr;
  }
};
