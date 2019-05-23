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

#include "net/grtm.h"

#include "net/grtm.h"

// ----------------------------------------------------------------------------
// GRtmWin32
// ----------------------------------------------------------------------------
struct GRtmWin32 : GRtm {
  friend struct GRtm;
protected:
  GRtmWin32();
  ~GRtmWin32 override();

  bool initialized_{false};
  bool init() override;
};
