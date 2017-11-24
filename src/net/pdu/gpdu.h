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

// ----------------------------------------------------------------------------
// GPdu
// ----------------------------------------------------------------------------
struct GPdu {
  typedef enum {
    Eth,
    Ip,
    Ip6,

    Tcp,
    Udp,
    Icmp,

    // TcpOption, // gilgil temp 2016.10.11
    TcpData,
    UdpData,

    // Dns, // gilgil temp 2016.10.11
    None
  } Type;

  typedef uint32_t Id;
  static const Id NULL_PDU_ID = 0;

  virtual ~GPdu() {}

  static const GPdu::Type staticType = None;
  virtual GPdu::Type pduType() { return staticType; }
  virtual GPdu::Id nextPduId() { return NULL_PDU_ID; }
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
  T* findFirst(GPdu::Type findType = T::staticType) {
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
  T* findNext(GPdu::Type findType = T::staticType) {
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
  T* findPrev(GPdu::Type findType = T::staticType) {
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
