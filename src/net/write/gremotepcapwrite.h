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

#include "gremotewrite.h"
#include "net/gnetinfo.h"

// ----------------------------------------------------------------------------
// GRemotePcapWrite
// ----------------------------------------------------------------------------
struct G_EXPORT GRemotePcapWrite : GRemoteWrite {
	Q_OBJECT

public:
	GRemotePcapWrite(QObject* parent = nullptr);
	~GRemotePcapWrite() override;

protected:
	bool doClose() override;

public:
	GPacket::Result write(GBuf buf) override;

public slots:
	GPacket::Result write(GPacket* packet) override;

signals:
	void written(GPacket* packet);

protected:
	GDemonClient* demonClient_{nullptr};
	GPacket::Dlt dlt_{GPacket::Null};
};
