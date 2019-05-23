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
// GRtmLinux
// ----------------------------------------------------------------------------
struct GRtmLinux : GRtm {
  friend struct GRtm;
protected:
  GRtmLinux();
  ~GRtmLinux() override;

  QStringList intfNames_;
  bool initialized_{false};
  void init() override;
};
