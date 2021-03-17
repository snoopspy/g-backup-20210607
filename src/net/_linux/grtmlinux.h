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

#include "net/grtm.h"

// ----------------------------------------------------------------------------
// GRtmLinux
// ----------------------------------------------------------------------------
struct G_EXPORT GRtmLinux : GRtm {
	friend struct GRtm;
	friend struct GNetInfo;

protected:
	GRtmLinux();
	~GRtmLinux() override;

protected:
	static bool checkA(char* buf, GRtmEntry* entry);
	static bool checkB(char* buf, GRtmEntry* entry);
	static bool checkC(char* buf, GRtmEntry* entry);
	static bool checkD(char* buf, GRtmEntry* entry);
	static bool decodeCidr(std::string cidr, GIp* dst, GIp* mask);
	static GIp numberToMask(int number);
};
