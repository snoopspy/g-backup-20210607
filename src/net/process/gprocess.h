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

#include "base/gstateobj.h"

// ----------------------------------------------------------------------------
// GProcess
// ----------------------------------------------------------------------------
struct GProcess : GStateObj {
  GProcess(QObject* parent = nullptr) : GStateObj(parent) {}
  ~GProcess() override {}
};
