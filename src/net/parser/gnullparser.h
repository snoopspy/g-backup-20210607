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
struct G_EXPORT GNullParser : GParser {
  Q_OBJECT

public:
  Q_INVOKABLE GNullParser(QObject* parent = nullptr) : GParser(parent) {}
  ~GNullParser() override {}

public:
  void parse(GPacket* packet) override;
};
