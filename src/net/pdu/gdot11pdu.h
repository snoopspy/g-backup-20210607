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

#include "gpdu.h"

// ----------------------------------------------------------------------------
// GDot11Parser
// ----------------------------------------------------------------------------
#include "net/parser/gparser.h"
struct GDot11Parser : GParser {
  Q_OBJECT

public:
  Q_INVOKABLE GDot11Parser(QObject* parent = nullptr) : GParser(parent) {}
};
