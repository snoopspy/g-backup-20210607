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

struct G {
  static const char* Version;
  static const unsigned long Timeout;
};

typedef void *pvoid;
typedef char *pchar;
typedef unsigned char gbyte, *pbyte;
#ifdef WIN32
  #ifdef _WIN64
    typedef unsigned __int64 size_t;
  #else // 32
    typedef unsigned int size_t;
  #endif
#endif
#ifdef linux
  typedef unsigned long size_t;
#endif

