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
  Q_PROPERTY(bool enabled MEMBER enabled_)

public:
  bool enabled_{true};

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
