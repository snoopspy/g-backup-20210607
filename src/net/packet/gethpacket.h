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

#include "gippacket.h"

// ----------------------------------------------------------------------------
// GEthPacket
// ----------------------------------------------------------------------------
struct G_EXPORT GEthPacket : GIpPacket {
	GEthPacket(QObject* parent = nullptr) : GIpPacket(parent) {
		dataLinkType_ = GPacket::Eth;
	}

	void parse() override;
	GPacket* clone(size_t extra  = 0) override;
};
typedef GEthPacket *PEthPacket;
