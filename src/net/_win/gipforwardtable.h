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

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include "base/gbase.h"

// ----------------------------------------------------------------------------
// GIpForwardTable
// ----------------------------------------------------------------------------
struct G_EXPORT GIpForwardTable {
private: // singleton
	GIpForwardTable();
	virtual ~GIpForwardTable();

protected:
	friend struct GRtmWin32;
	PMIB_IPFORWARDTABLE ipForwardTable_{nullptr};

public:
	static GIpForwardTable& instance() {
		static GIpForwardTable ipForwardTable;
		return ipForwardTable;
	}
};


