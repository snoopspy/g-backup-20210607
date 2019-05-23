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
struct G_EXPORT GPacketDebugger : GStateObj {
  Q_OBJECT
  Q_PROPERTY(bool debug MEMBER debug_)

public:
  bool debug_{true};

public:
  Q_INVOKABLE GPacketDebugger(QObject* parent = nullptr) : GStateObj(parent) {}
  ~GPacketDebugger() override {}

protected:
  bool doOpen() override { return true; }
  bool doClose() override { return true; }

public slots:
  void debug(GPacket* packet);

signals:
  void debugged(GPacket* packet);
};
