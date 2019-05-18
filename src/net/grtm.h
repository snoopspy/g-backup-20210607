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

#include <QList>
#include "net/gnetintf.h"

// ----------------------------------------------------------------------------
// GRtmEntry
// ----------------------------------------------------------------------------
struct GRtmEntry {
  GIp dst_{uint32_t(0)};
  GIp mask_{uint32_t(0)};
  GIp gateway_{uint32_t(0)};
  GNetIntf* intf_{nullptr};
  int metric_{0};
};

// ----------------------------------------------------------------------------
// GRtm
// ----------------------------------------------------------------------------
struct GRtm : QList<GRtmEntry> {
protected: // inherited singleton
  GRtm();
  virtual ~GRtm();

public:
  virtual bool init() = 0;
  GRtmEntry* getBestEntry(GIp ip);
  GIp findGateway(QString intfName, GIp ip);
  static GRtm& instance();
};
