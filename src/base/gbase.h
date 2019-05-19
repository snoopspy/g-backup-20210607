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

#include <cstddef> // for size_t
#include <cstdint>

struct G {
  static const char* Version;
  static const unsigned long Timeout;
};

typedef void    *pvoid;
typedef char    *pchar;
typedef uint8_t  gbyte, *pbyte;
typedef size_t   gsize, *psize;
