// ----------------------------------------------------------------------------
//
// G Library
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#include "gpcapdevicewrite.h"

// ----------------------------------------------------------------------------
// GPcapDeviceWriteEth
// ----------------------------------------------------------------------------
struct G_EXPORT GPcapDeviceWriteEth : GPcapDeviceWrite {
	Q_OBJECT
	Q_PROPERTY(QString smac MEMBER smacStr_)
	Q_PROPERTY(QString dmac MEMBER dmacStr_)
	Q_PROPERTY(quint16 type MEMBER type_)

public:
	QString smacStr_{"000000-000000"};
	QString dmacStr_{"000000-000000"};
	uint16_t type_{GEthHdr::Ip4};

public:
	Q_INVOKABLE GPcapDeviceWriteEth(QObject* parent = nullptr) : GPcapDeviceWrite(parent) {
		dataLinkType_ = GPacket::Eth;
	}
	~GPcapDeviceWriteEth() override { close(); }

protected:
	bool doOpen() override;
	bool doClose() override;

protected:
	GMac smac_;
	GMac dmac_;
	static constexpr int MAXBUF = 65536;
	gbyte temp_[MAXBUF];

public slots:
	GPacket::Result writeEth(GPacket* packet);
};
