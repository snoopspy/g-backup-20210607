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
// GSyncPcapDevice
// ----------------------------------------------------------------------------
struct GSyncPcapDevice : GPcapDevice {
  Q_OBJECT

public:
  Q_INVOKABLE GSyncPcapDevice(QObject* parent = nullptr) : GPcapDevice(parent) {
    autoRead_ = false;
  }
};
