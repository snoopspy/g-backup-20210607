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

#include "base/gobj.h"
#include "net/capture/gcapture.h"

// ----------------------------------------------------------------------------
// GParser
// ----------------------------------------------------------------------------
struct GParser : GObj {
  GParser(QObject* parent = nullptr) : GObj(parent) {}
  ~GParser() override {}

  virtual void parse(GPacket* packet);

  static GParser* getDefaultParser(GCapture::DataLinkType dataLinkType);
};
