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

	std::string error_;

	std::string ip_;
	uint16_t port_;

	int sd_{0};

	char readBuffer_[MaxBufferSize]; // used in pcapRead
	bool connect();
	bool disconnect();

	GetInterfaceListRep getInterfaceList();
	GetRtmRep getRtm();
	PcapOpenRep pcapOpen(std::string filter, std::string intfName, int32_t snapLen, int32_t flags, int32_t readTimeout, int32_t waitTimeout, bool captureThread);
	void pcapClose();
	PcapRead pcapRead();

	static GDemonClient* instance(std::string ip, uint16_t port);
};
