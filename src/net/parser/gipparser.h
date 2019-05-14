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
// GIpParser
// ----------------------------------------------------------------------------
struct GIpParser : GParser {
  Q_OBJECT

public:
  Q_INVOKABLE GIpParser(QObject* parent = nullptr) : GParser(parent) {}
  ~GIpParser() override {}

public:
  void parse(GPacket* packet) override;
};
