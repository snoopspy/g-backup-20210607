// ----------------------------------------------------------------------------
//
// G Library
//
// http://gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

#include "gremotepcapdevice.h"

// ----------------------------------------------------------------------------
// GSyncRemotePcapDevice
// ----------------------------------------------------------------------------
struct G_EXPORT GSyncRemotePcapDevice : GRemotePcapDevice {
	Q_OBJECT

public:
	Q_INVOKABLE GSyncRemotePcapDevice(QObject* parent = nullptr) : GRemotePcapDevice(parent) {
		autoRead_ = false;
	}
};
