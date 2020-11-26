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

#include "gpacket.h"

// ----------------------------------------------------------------------------
// GDot11Packet
// ----------------------------------------------------------------------------
struct G_EXPORT GDot11Packet : GPacket {
	GDot11Packet(QObject* parent = nullptr) : GPacket(parent) {
		dataLinkType_ = GPacket::Dot11;
	}

	void parse() override;
	GPacket* clone(size_t extra  = 0) override;
};
typedef GDot11Packet *PDot11Packet;
