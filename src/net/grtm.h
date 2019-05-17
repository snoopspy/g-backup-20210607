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
#include "net/gip.h"

// ----------------------------------------------------------------------------
// GRtmEntry
// ----------------------------------------------------------------------------
struct GRtmEntry {
  GIp dst_{uint32_t(0)};
  GIp mask_{uint32_t(0)};
  GIp gateway_{uint32_t(0)};
  QString intf_;
  int metric_{0};
};

// ----------------------------------------------------------------------------
// GRtm
// ----------------------------------------------------------------------------
struct GRtm : QList<GRtmEntry> {
private:
  GRtm();
  virtual ~GRtm();

public:
  bool loadFromSystem();
  GRtmEntry* getBestEntry(GIp ip);
#ifdef Q_OS_LINUX
  GIp findGateway(QString intf);
#endif
  static GRtm& instance();
};
