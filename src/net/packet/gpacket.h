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
struct GPacket {
  GPacket();
  GPacket(GCapture* capture);
  void clear();

  GCapture* capture_;

  struct timeval ts_;
  u_char* buf_;
  size_t len_;

  GPdus pdus_;
};
