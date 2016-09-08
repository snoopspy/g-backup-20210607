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
// GDot11Parser
// ----------------------------------------------------------------------------
struct GDot11Parser : GParser {
  void parse(GPacket* packet) override;

  static GDot11Parser& instance() {
    static GDot11Parser dot11Parser;
    return dot11Parser;
  }
};
