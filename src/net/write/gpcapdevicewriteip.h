// ----------------------------------------------------------------------------
//
// G Library
//
// http://gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#include "gpcapdevicewrite.h"

// ----------------------------------------------------------------------------
// GPcapDeviceWriteIp
// ----------------------------------------------------------------------------
struct G_EXPORT GPcapDeviceWriteIp : GPcapDeviceWrite {
	Q_OBJECT

public:
	Q_INVOKABLE GPcapDeviceWriteIp(QObject* parent = nullptr) : GPcapDeviceWrite(parent) {
		dataLinkType_ = GPacket::Ip;
	}
	~GPcapDeviceWriteIp() override { close(); }

protected:
	bool doOpen() override;
	bool doClose() override;

public slots:
	GPacket::Result write(GPacket* packet);
};
