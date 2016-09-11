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

protected:
  GPdus* pdus_;

public:
  GPdu* first() { return pdus_->first(); }
  GPdu* next() { return pdus_->next(); }
  GPdu* prev() { return pdus_->prev(); }
  GPdu* findFirst(GPdu::Type type) { return pdus_->findFirst(type); }
  GPdu* findNext(GPdu::Type type) { return pdus_->findNext(type); }
  GPdu* findPrev(GPdu::Type type) { return pdus_->findPrev(type); }
};
