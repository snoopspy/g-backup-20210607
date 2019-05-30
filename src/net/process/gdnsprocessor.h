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
#include "gdns.h"

// ----------------------------------------------------------------------------
// GDnsProcessor
// ----------------------------------------------------------------------------
struct G_EXPORT GDnsProcessor : GStateObj {
  Q_OBJECT
  Q_PROPERTY(quint16 port MEMBER port_)

public:
  uint16_t port_{53};

public:
  Q_INVOKABLE GDnsProcessor(QObject* parent = nullptr) : GStateObj(parent) {}
  ~GDnsProcessor() override {}

protected:
  bool doOpen() override { return true; }
  bool doClose() override { return true; }

public slots:
  void process(GPacket* packet);

signals:
  void dnsCaptured(GPacket* packet, GDns* dns);
};
