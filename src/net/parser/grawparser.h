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

#include "gparser.h"

// ----------------------------------------------------------------------------
// GRawParser
// ----------------------------------------------------------------------------
struct GRawParser : GParser {
  virtual void parse(GPacket* packet);

  static GRawParser& instance() {
    static GRawParser rawParser;
    return rawParser;
  }
};
