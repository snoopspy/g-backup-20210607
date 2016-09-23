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

#include "net/capture/gpcapdevice.h"

// ----------------------------------------------------------------------------
// GPcapDeviceWriter
// ----------------------------------------------------------------------------
struct GPcapDeviceWriter : GPcapDevice {
  Q_OBJECT

public:
  Q_INVOKABLE GPcapDeviceWriter(QObject* parent = nullptr);
  ~GPcapDeviceWriter() override;

public slots:
  Result write(GPacket* packet) override;
};
