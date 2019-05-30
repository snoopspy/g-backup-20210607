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

#include "gnet.h"

// ----------------------------------------------------------------------------
// GIp6
// ----------------------------------------------------------------------------
struct G_EXPORT GIp6 final {
public:
  static const int SIZE = 16;

protected:
  gbyte ip6_[SIZE];

public:
  //
  // constructor
  //
  GIp6() {}
  GIp6(const GIp6& rhs) { memcpy(ip6_, rhs.ip6_, SIZE); }
  GIp6(const gbyte* rhs) { memcpy(ip6_, rhs, SIZE); }
  GIp6(const char* rhs);
  GIp6(const QString& rhs) : GIp6(qPrintable(rhs)) {}

  //
  // casting operator
  //
  operator gbyte*() const { return const_cast<gbyte*>(ip6_); } // default casting operator
  explicit operator const char*() const { return qPrintable(QString(*this)); }
  explicit operator QString() const;

  //
  // comparison operator
  //
  bool operator == (const GIp6& rhs) const   { return memcmp(ip6_, rhs.ip6_, SIZE) == 0; }
  bool operator != (const GIp6& rhs) const   { return memcmp(ip6_, rhs.ip6_, SIZE) != 0; }
  bool operator <  (const GIp6& rhs) const   { return memcmp(ip6_, rhs.ip6_, SIZE) <  0; }
  bool operator >  (const GIp6& rhs) const   { return memcmp(ip6_, rhs.ip6_, SIZE) >  0; }
  bool operator <= (const GIp6& rhs) const   { return memcmp(ip6_, rhs.ip6_, SIZE) <= 0; }
  bool operator >= (const GIp6& rhs) const   { return memcmp(ip6_, rhs.ip6_, SIZE) >= 0; }
  bool operator == (const u_char* rhs) const { return memcmp(ip6_, rhs,      SIZE) == 0; }

public:
  void clear() { memset(ip6_, 0, SIZE); }

  bool isLocalHost() {
    return true; // gilgil temp 2019.05.11
  }

  bool isBroadcast() {
    return true; // gilgil temp 2019.05.11
  }

  bool isMulticast() {
    return true; // gilgil temp 2019.05.11
  }
};
