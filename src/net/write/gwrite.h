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

#include "base/gstateobj.h"
#include "net/packet/gpacket.h"

// ----------------------------------------------------------------------------
// GWrite
// ----------------------------------------------------------------------------
struct G_EXPORT GWrite : GStateObj {

public:
	Q_INVOKABLE GWrite(QObject* parent = nullptr) : GStateObj(parent) {}
	~GWrite() override {}

	GPacket::DataLinkType dataLinkType() { return dataLinkType_; }

protected:
	GPacket::DataLinkType dataLinkType_{GPacket::Null};

public:
	virtual GPacket::Result write(GPacket* packet);
	virtual GPacket::Result write(GBuf buf);
};

