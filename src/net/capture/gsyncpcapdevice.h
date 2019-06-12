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

#include "gpcapdevice.h"

// ----------------------------------------------------------------------------
// GSyncPcapDevice
// ----------------------------------------------------------------------------
struct G_EXPORT GSyncPcapDevice : GPcapDevice {
	Q_OBJECT

public:
	Q_INVOKABLE GSyncPcapDevice(QObject* parent = nullptr) : GPcapDevice(parent) {
		autoRead_ = false;
	}
};
