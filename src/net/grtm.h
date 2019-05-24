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
struct G_EXPORT GRtmEntry {
  friend struct GRtm;
#ifdef Q_OS_LINUX
  friend struct GRtmLinux;
#endif
#ifdef Q_OS_WIN
  friend struct GRtmWin32;
#endif
public:
  GIp dst() const { return dst_; }
  GIp mask() const { return mask_; }
  GIp gateway() const { return gateway_; }
  int metric() const { return metric_; }
  GNetIntf* intf() const;

protected:
  GIp dst_{uint32_t(0)};
  GIp mask_{uint32_t(0)};
  GIp gateway_{uint32_t(0)};
  int metric_{0};
  GNetIntf* intf_{nullptr};
};

// ----------------------------------------------------------------------------
// GRtm
// ----------------------------------------------------------------------------
struct __declspec(dllexport) GRtm : QList<GRtmEntry> {
  friend struct GRtmEntry;
protected: // inherited singleton
  GRtm();
  virtual ~GRtm();
  virtual void init() = 0;

public:
  GRtmEntry* getBestEntry(GIp ip);
  GIp findGateway(QString intfName, GIp ip);
  static GRtm& instance();
};
