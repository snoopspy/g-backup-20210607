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

#include "gdemon.h"

// ----------------------------------------------------------------------------
// GDemonClient
// ----------------------------------------------------------------------------
struct GDemonClient : GDemon {
	GDemonClient(std::string ip, uint16_t port);
	~GDemonClient() override;

	std::string ip_;
	uint16_t port_;

	std::string error_;
	int sd_{0};

	bool connect();
	bool disconnect();

	InterfaceList getInterfaceList();
	Rtm getRtm();

	static GDemonClient* instance(std::string ip, uint16_t port);
};
