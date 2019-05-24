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

#include "net/capture/gnetfilter.h"

#ifdef Q_OS_LINUX

// ----------------------------------------------------------------------------
// GSyncNetFilter
// ----------------------------------------------------------------------------
struct G_EXPORT GSyncNetFilter : GNetFilter {
  Q_OBJECT

public:
  Q_INVOKABLE GSyncNetFilter(QObject* parent = nullptr);
  ~GSyncNetFilter() override;

protected:
  bool doOpen() override;
  bool doClose() override;

public:
  GPacket::Result read(GPacket* packet) override;
  GPacket::Result write(GPacket* packet) override;
  GPacket::Result write(GBuf* buf) override;
  GPacket::Result relay(GPacket* packet) override;
};

#endif
