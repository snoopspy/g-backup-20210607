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
// GRawParser
// ----------------------------------------------------------------------------
struct GRawParser : GParser {
  Q_OBJECT

public:
  Q_INVOKABLE GRawParser(QObject* parent = nullptr) : GParser(parent) {}
};
