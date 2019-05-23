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
struct G_EXPORT GParserFactory : GObj {
private:
  GParserFactory();
  ~GParserFactory() override;

public:
  static GParser* getDefaultParser(GPacket::DataLinkType dataLinkType);
  static GParserFactory& instance();

protected:
  GParser* ethParser_;
  GParser* ipParser_;
  GParser* dot11Parser_;
  GParser* nullParser_;
};
