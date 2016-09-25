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
#include "net/packet/gpacket.h"

// ----------------------------------------------------------------------------
// GPacketDebugger
// ----------------------------------------------------------------------------
struct GPacketDebugger : GStateObj {
  Q_OBJECT

public:
  Q_INVOKABLE GPacketDebugger(QObject* parent = nullptr) : GStateObj(parent) {}
  ~GPacketDebugger() override {}

protected:
  bool doOpen() override;
  bool doClose() override;

public slots:
  void debug(GPacket* packet);
};
