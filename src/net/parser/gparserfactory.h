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
// GParserFactory
// ----------------------------------------------------------------------------
struct GParserFactory {
private:
  GParserFactory();
  virtual  ~GParserFactory();

public:
  static GParserFactory& instance();
  static void init();
  static GParser* getDefaultParser(GCapture::DataLinkType dataLinkType);

protected:
  GParser* root_;
};
