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
struct G_EXPORT GDot11Parser : GParser {
  Q_OBJECT

public:
  Q_INVOKABLE GDot11Parser(QObject* parent = nullptr) : GParser(parent) {}
  ~GDot11Parser() override {}

public:
  void parse(GPacket* packet) override;
};
