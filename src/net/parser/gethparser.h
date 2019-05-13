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

#include "gipparser.h"

// ----------------------------------------------------------------------------
// GEthParser
// ----------------------------------------------------------------------------
struct GEthParser : GIpParser {
  Q_OBJECT

public:
  Q_INVOKABLE GEthParser(QObject* parent = nullptr) : GIpParser(parent) {}
  ~GEthParser() override {}

public:
  bool parse(GPacket* packet) override;
};
