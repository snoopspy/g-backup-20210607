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

#include "gremotecapture.h"
#include "net/gnetinfo.h"

// ----------------------------------------------------------------------------
// GRemotePcapCapture
// ----------------------------------------------------------------------------
struct G_EXPORT GRemotePcapCapture : GRemoteCapture {
	Q_OBJECT
	Q_PROPERTY(QString filter MEMBER filter_)

public:
	QString filter_{""};

	GRemotePcapCapture(QObject* parent = nullptr) : GRemoteCapture(parent) {}
	~GRemotePcapCapture() override { close(); }

protected:
	bool doOpen() override;
	bool doClose() override;

public:
	GPacket::Result read(GPacket* packet) override;
	GPacket::Result write(GBuf buf) override;
	GPacket::Result write(GPacket* packet) override;
	GPacket::Result relay(GPacket* packet) override;

	GPacket::Dlt dlt() override { return dlt_; }
	PathType pathType() override { return OutOfPath; }

protected:
	GDemonClient* demonClient_{nullptr};
	GPacket::Dlt dlt_{GPacket::Null};
};
