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

#include <winsock2.h>
#include <iphlpapi.h>

#include "base/gbase.h"

// ----------------------------------------------------------------------------
// GIpAdapterInfo
// ----------------------------------------------------------------------------
struct G_EXPORT GIpAdapterInfo {
private: // singleton
	GIpAdapterInfo();
	virtual ~GIpAdapterInfo();

protected:
	PIP_ADAPTER_INFO pAdapterInfo_{nullptr};

public:
	PIP_ADAPTER_INFO findByAdapterName(char* value);
	PIP_ADAPTER_INFO findByComboIndex(DWORD comboIndex);

	static GIpAdapterInfo& instance() {
		static GIpAdapterInfo ipAdapterInfo;
		return ipAdapterInfo;
	}
};

