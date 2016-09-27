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

#include "net/packet/gpacket.h"
#include "gprocess.h"

// ----------------------------------------------------------------------------
// GPacketDebugger
// ----------------------------------------------------------------------------
struct GPacketDebugger : GProcess {
  Q_OBJECT

public:
  Q_INVOKABLE GPacketDebugger(QObject* parent = nullptr) : GProcess(parent) {}
  ~GPacketDebugger() override {}

protected:
  bool doOpen() override { return true; }
  bool doClose() override { return true; }

public slots:
  void debug(GPacket* packet);
};
