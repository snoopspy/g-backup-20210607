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
// GTimeStampDelay
// ----------------------------------------------------------------------------
struct GTimeStampDelay : GStateObj {
  Q_OBJECT
  Q_PROPERTY(double speed MEMBER speed_) // ratio

public:
  double speed_{1.};

protected:
  QWaitCondition wc_;
  QMutex m_;
  QElapsedTimer et_;
  qint64 lastClock_;
  qint64 lastTs_;

public:
  Q_INVOKABLE GTimeStampDelay(QObject* parent = nullptr) : GStateObj(parent) {}
  ~GTimeStampDelay() override {}

protected:
  bool doOpen() override;
  bool doClose() override;

public slots:
  void delay(GPacket* packet);

signals:
  void delayed(GPacket* packet);
};
