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

#include <sys/time.h>
#include <QVector>
#include "net/pdu/gpdu.h"

// ----------------------------------------------------------------------------
// GPacket
// ----------------------------------------------------------------------------
struct GCapture;
struct GPdu;
struct GPdus;
struct GPacket {
  friend struct GParser;

public:
  GPacket();
  GPacket(GCapture* capture);
  virtual ~GPacket();

  void clear();

  GCapture* capture_;

  struct timeval ts_;
  u_char* buf_;
  size_t len_;

public: // protected: // gilgil temp 2016.09.12
  GPdus pdus_;

public:
  GPdu* first() { return pdus_.first(); }
  GPdu* next() { return pdus_.next(); }
  GPdu* prev() { return pdus_.prev(); }

  template <typename T>
  T* findFirst() { return pdus_.findFirst<T>(); }

  template <typename T>
  T* findNext() { return pdus_.findNext<T>(); }

  template <typename T>
  T* findPrev() { return pdus_.findPrev<T>(); }
};
