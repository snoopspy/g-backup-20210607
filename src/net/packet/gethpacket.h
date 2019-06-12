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
};
typedef GEthPacket *PEthPacket;
