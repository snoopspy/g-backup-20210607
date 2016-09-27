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
#include "base/gstateobj.h"

// ----------------------------------------------------------------------------
// GBlock
// ----------------------------------------------------------------------------
struct GBlock : GStateObj {
  Q_OBJECT
  Q_PROPERTY(bool enabled MEMBER enabled_)

public:
  bool enabled_{true};

public:
  Q_INVOKABLE GBlock(QObject* parent = nullptr) : GStateObj(parent) {}
  ~GBlock() override {}

protected:
  bool doOpen() override { return true; }
  bool doClose() override { return true; }

public slots:
  void block(GPacket* packet);
};
