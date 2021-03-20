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
#include "net/capture/gsyncpcapdevice.h"

// ----------------------------------------------------------------------------
// GPcapDeviceWriteEthAutoMac
// ----------------------------------------------------------------------------
struct G_EXPORT GPcapDeviceWriteEthAutoMac : GPcapDeviceWrite {
	Q_OBJECT
	Q_PROPERTY(quint16 type MEMBER type_)

public:
	uint16_t type_{GEthHdr::Ip4};

public:
	Q_INVOKABLE GPcapDeviceWriteEthAutoMac(QObject* parent = nullptr) : GPcapDeviceWrite(parent) {
		dataLinkType_ = GPacket::Eth;
	}
	~GPcapDeviceWriteEthAutoMac() override { close(); }

protected:
	bool doOpen() override;
	bool doClose() override;

protected:
	GMac myMac_;
	GSyncPcapDevice* device_{nullptr};
	static constexpr int MAXBUF = 65536;
	gbyte temp_[MAXBUF];

public slots:
	GPacket::Result writeEth(GPacket* packet);
};
