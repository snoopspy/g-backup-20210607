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

#include "gdemon.h"

// ----------------------------------------------------------------------------
// GDemonClient
// ----------------------------------------------------------------------------
struct GDemonClient : GDemon {
	GDemonClient();
	~GDemonClient() override;

	std::string error_;
	int sd_{0};

	bool connect(std::string ip = "", uint16_t port = 0);
	bool disconnect();

	InterfaceList getInterfaceList();
	Rtm getRtm();

	static GDemonClient& instance();
};
