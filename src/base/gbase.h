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

#ifdef WIN32
  #if defined(G_BUILD)
    #define G_EXPORT __declspec(dllexport)
  #else
    #define G_EXPORT __declspec(dllimport)
  #endif
#endif
#ifdef linux
  #define G_EXPORT
#endif

typedef unsigned long GDuration;

struct G_EXPORT G {
  static const GDuration Timeout;
  static const char* version();
  static const char* pcap_lib_version();
};

#define GDEBUG_CTOR { qDebug() << "constructor  " << pvoid(this); }
#define GDEBUG_DTOR { qDebug() << "destructor " << pvoid(this); }
