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

#include "grawparser.h"

// ----------------------------------------------------------------------------
// GEthParser
// ----------------------------------------------------------------------------
struct GEthParser : GRawParser {
  void parse(GPacket* packet) override;

  static GEthParser& instance() {
    static GEthParser ethParser;
    return ethParser;
  }
};
