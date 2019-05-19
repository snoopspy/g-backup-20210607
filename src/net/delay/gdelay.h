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
// GDelay
// ----------------------------------------------------------------------------
struct GDelay : GStateObj {
  Q_OBJECT
  Q_PROPERTY(ulong duration MEMBER duration_) // msec

public:
  ulong duration_{1000};

protected:
  QWaitCondition wc_;
  QMutex m_;
  QElapsedTimer et_;
  qint64 last_;

public:
  Q_INVOKABLE GDelay(QObject* parent = nullptr) : GStateObj(parent) {}
  ~GDelay() override {}

protected:
  bool doOpen() override;
  bool doClose() override;

public slots:
  void delay(GPacket* packet);

signals:
  void delayed(GPacket* packet);
};
