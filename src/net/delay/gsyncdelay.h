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
// GSyncDelay
// ----------------------------------------------------------------------------
struct G_EXPORT GSyncDelay : GStateObj {
  Q_OBJECT
  Q_PROPERTY(ulong duration MEMBER duration_) // msec
  Q_PROPERTY(double speed MEMBER speed_) // ratio

public:
  ulong duration_{1000};
  double speed_{1.};

protected:
  GWaitEvent we_;
  QElapsedTimer et_;
  qint64 lastClock_;

public:
  Q_INVOKABLE GSyncDelay(QObject* parent = nullptr) : GStateObj(parent) {}
  ~GSyncDelay() override { close(); }

protected:
  bool doOpen() override;
  bool doClose() override;

public slots:
  void delay(GPacket* packet);

signals:
  void delayed(GPacket* packet);
};
