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
#include "base/sys/gwaitevent.h"

// ----------------------------------------------------------------------------
// GDelay
// ----------------------------------------------------------------------------
struct G_EXPORT GDelay : GStateObj {
  Q_OBJECT
  Q_PROPERTY(ulong duration MEMBER duration_) // msec

public:
  ulong duration_{1000};

protected:
  GWaitEvent we_;

public:
  Q_INVOKABLE GDelay(QObject* parent = nullptr) : GStateObj(parent) {}
  ~GDelay() override { close(); }

protected:
  bool doOpen() override;
  bool doClose() override;

public slots:
  void sleep(GPacket* packet);

signals:
  void sleeped(GPacket* packet);
};
