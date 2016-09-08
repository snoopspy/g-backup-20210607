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
// GNullParser
// ----------------------------------------------------------------------------
struct GNullParser : GParser {
  void parse(GPacket* packet) override;

  static GNullParser& instance() {
    static GNullParser nullParser;
    return nullParser;
  }
};
