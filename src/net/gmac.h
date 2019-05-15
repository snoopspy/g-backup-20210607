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

#include <QString>

#ifdef Q_OS_WIN
  #include <winsock2.h> // for u_char
#endif

// ----------------------------------------------------------------------------
// GMac
// ----------------------------------------------------------------------------
struct GMac final {
public:
  static const int SIZE = 6;

protected:
  u_char mac_[SIZE];

public:
  //
  // constructor
  //
  GMac() {}
  GMac(const GMac& rhs) { memcpy(this->mac_, rhs.mac_, SIZE); }
  GMac(const u_char* rhs) { memcpy(this->mac_, rhs, SIZE); }
  GMac(const char* rhs);
  GMac(const QString& rhs) : GMac(qPrintable(rhs)) {}

  //
  // casting operator
  //
  operator u_char*() const { return const_cast<u_char*>(mac_); } // default casting operator
  explicit operator const char*() const { return qPrintable(QString(*this)); }
  explicit operator QString() const;

  //
  // comparison operator
  //
  bool operator == (const GMac& rhs) const   { return memcmp(mac_, rhs.mac_, SIZE) == 0; }
  bool operator != (const GMac& rhs) const   { return memcmp(mac_, rhs.mac_, SIZE) != 0; }
  bool operator <  (const GMac& rhs) const   { return memcmp(mac_, rhs.mac_, SIZE) <  0; }
  bool operator >  (const GMac& rhs) const   { return memcmp(mac_, rhs.mac_, SIZE) >  0; }
  bool operator <= (const GMac& rhs) const   { return memcmp(mac_, rhs.mac_, SIZE) <= 0; }
  bool operator >= (const GMac& rhs) const   { return memcmp(mac_, rhs.mac_, SIZE) >= 0; }
  bool operator == (const u_char* rhs) const { return memcmp(mac_, rhs,      SIZE) == 0; }

public:
  void clear() {
    *this = cleanMac();
  }

  bool isClean() const {
    return *this == cleanMac();
  }

  bool isBroadcast() const { // FF:FF:FF:FF:FF:FF
    return *this == broadcastMac();
  }

  bool isMulticast() const { // 01:00:5E:0*
    return mac_[0] == 0x01 && mac_[1] == 0x00 && mac_[2] == 0x5E && (mac_[3] & 0x80) == 0x00;
  }

  static GMac randomMac();
  static GMac& cleanMac();
  static GMac& broadcastMac();
};
