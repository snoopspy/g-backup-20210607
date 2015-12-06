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

// ----------------------------------------------------------------------------
// GMac
// ----------------------------------------------------------------------------
#pragma pack(push, 1)
class GMac
{
public:
  static const int SIZE = 6;

protected:
  uint8_t value[SIZE];

public:
  GMac()                     {                                   }
  GMac(const uint8_t* value) { memcpy(this->value, value, SIZE); }
  GMac(const QString s);
  GMac(const char* s)        { *this = QString(s);               }

  operator uint8_t*() const  { return (uint8_t*)value;           } // cast operator
  explicit operator const char*() const { return qPrintable((QString)*this); }
  explicit operator QString() const;

  bool operator == (const GMac& rhs) const   { return memcmp(value, rhs.value, SIZE) == 0; }
  bool operator != (const GMac& rhs) const   { return memcmp(value, rhs.value, SIZE) != 0; }
  bool operator <  (const GMac& rhs) const   { return memcmp(value, rhs.value, SIZE) <  0; }
  bool operator >  (const GMac& rhs) const   { return memcmp(value, rhs.value, SIZE) >  0; }
  bool operator <= (const GMac& rhs) const   { return memcmp(value, rhs.value, SIZE) <= 0; }
  bool operator >= (const GMac& rhs) const   { return memcmp(value, rhs.value, SIZE) >= 0; }
  bool operator == (const uint8_t* rhs) const { return memcmp(value, rhs,       SIZE) == 0; }

public:
  void clear() { *this = cleanMac(); }

public:
  bool isClean() const     { return *this == cleanMac();       }
  bool isBroadcast() const { return *this == broadcastMac();   } // FF:FF:FF:FF:FF:FF
  bool isMulticast() const { return value[0] == 0x01 && value[1] == 0x00 && value[2] == 0x5E && (value[3] & 0x80) == 0x00; } // 01:00:5e:0*

  static GMac  randomMac();
  static GMac& cleanMac();
  static GMac& broadcastMac();
};
#pragma pack(pop)
