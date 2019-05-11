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
  GIp(const char* rhs) { *this = rhs; }
  GIp(const QString& rhs) { *this = rhs; }
  GIp(const struct in_addr& rhs) { *this = rhs; }

  //
  // casting operator
  //
  operator uint32_t() const { return ip_; } // default casting operator
  explicit operator const char*() const { return qPrintable(QString(*this)); }
  explicit operator const QString() const;

  //
  // assignment operator
  //
  GIp& operator = (const GIp& rhs) { ip_ = rhs.ip_; return *this; }
  GIp& operator = (const uint32_t rhs) { ip_ = rhs; return *this; }
  GIp& operator = (const char* rhs);
  GIp& operator = (const QString& rhs) { *this = qPrintable(rhs); return *this; }
  GIp& operator = (const struct in_addr& rhs) { ip_ = ntohl(rhs.s_addr);  return *this; }

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
