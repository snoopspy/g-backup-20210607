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
#include <map>
#include <utility>

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

// ----------------------------------------------------------------------------
// GDemonClientMapKey
// ----------------------------------------------------------------------------
struct GDemonClientMapKey {
	GDemonClientMapKey(std::string ip, uint16_t port) : ip_(ip), port_(port) {}
	std::string ip_;
	uint16_t port_;
	bool operator<(const GDemonClientMapKey &r) const {
		if (ip_ < r.ip_) return true;
		if (ip_ > r.ip_) return false;
		if (port_ < r.port_) return true;
		if (port_ > r.port_) return false;
		return false;
	}
};

// ----------------------------------------------------------------------------
// GDemonClientMap
// ----------------------------------------------------------------------------
struct GDemonClientMap : std::map<GDemonClientMapKey, GDemonClient*> {
	friend struct GDemonClient;

protected: // singleton
	virtual ~GDemonClientMap();
};
