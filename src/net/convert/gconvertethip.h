// ----------------------------------------------------------------------------
//
// G Library
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#include "base/gstateobj.h"
#include "net/packet/gpacket.h"

// ----------------------------------------------------------------------------
// GConvertEthIp
// ----------------------------------------------------------------------------
struct G_EXPORT GConvertEthIp : GStateObj {
	Q_OBJECT

public slots:
	bool convert(GPacket* packet);

signals:
	void converted(GPacket* packet);
};
