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
#include "net/capture/gcapture.h"

// ----------------------------------------------------------------------------
// GParserMap
// ----------------------------------------------------------------------------
struct GParserMap {
private:
  GParserMap();
  virtual  ~GParserMap();

public:
  static GParserMap& instance();
  static GParser* getDefaultParser(GCapture::DataLinkType dataLinkType);

protected:
  GParser* root_;
};
