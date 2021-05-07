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

#include "net/grtm.h"

// ----------------------------------------------------------------------------
// GRtmLinux
// ----------------------------------------------------------------------------
struct G_EXPORT GRtmLinux : GRtm {
	friend struct GNetInfo;

protected: // singleton
	GRtmLinux();
	~GRtmLinux() override {}

protected:
	static bool checkA(char* buf, GRtmEntry* entry);
	static bool checkB(char* buf, GRtmEntry* entry);
	static bool checkC(char* buf, GRtmEntry* entry);
	static bool checkD(char* buf, GRtmEntry* entry);
	static bool decodeCidr(std::string cidr, uint32_t* dst, uint32_t* mask);
	static uint32_t numberToMask(int number);
};
