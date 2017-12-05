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
struct GParserFactory : GObj {
private:
  GParserFactory();
  virtual  ~GParserFactory();

public:
  static GParserFactory& instance();
  static void init();
  static GParser* getDefaultParser(GPacket::DataLinkType dataLinkType);

protected:
  GParser* ethParser_;
  GParser* dot11Parser_;
  GParser* ipParser_;
  GParser* nullParser_;
};
