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
// GNullParser
// ----------------------------------------------------------------------------
struct GNullParser : GParser {
  Q_OBJECT

public:
  Q_INVOKABLE GNullParser(QObject* parent = nullptr) : GParser(parent) {}
};
