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

// ----------------------------------------------------------------------------
// GPacket
// ----------------------------------------------------------------------------
struct GCapture;
struct GPdus;
struct GPacket {
  GPacket();
  GPacket(GCapture* capture);
  virtual ~GPacket();

  void clear();

  GCapture* capture_;

  struct timeval ts_;
  u_char* buf_;
  size_t len_;

  GPdus* pdus_;
};
