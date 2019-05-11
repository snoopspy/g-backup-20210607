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

//#include <netinet/in.h> // gilgil temp 2019.05.11
#include <QString>

// ----------------------------------------------------------------------------
// GIp
// ----------------------------------------------------------------------------
struct GIp final {
protected:
  uint32_t ip_;

public:
  //
  // constructor
  //
  GIp() {}
  GIp(const GIp& rhs) : ip_(rhs.ip_) {}
  GIp(const uint32_t rhs) : ip_(rhs) {}
  GIp(const char* rhs);
  GIp(const QString& rhs) { GIp(qPrintable(rhs)); }

  //
  // casting operator
  //
  operator uint32_t() const { return ip_; } // default casting operator
  explicit operator const char*() const { return qPrintable(QString(*this)); }
  explicit operator QString() const;

public:
  void clear() { ip_ = 0; }

  bool isLocalHost() const { // 127.*.*.*
    uint8_t prefix = (ip_ & 0xFF000000) >> 24;
    return prefix == 0x7F;
  }

  bool isBroadcast() const { // 255.255.255.255
    return ip_ == 0xFFFFFFFF;
  }

  bool isMulticast() const { // 224.0.0.0 ~ 239.255.255.255
    uint8_t prefix = (ip_ & 0xFF000000) >> 24;
    return prefix >= 0xE0 && prefix < 0xF0;
  }
};
