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

#include "net/write/gwrite.h"

// ----------------------------------------------------------------------------
// GRawSocketWrite
// ----------------------------------------------------------------------------
struct G_EXPORT GRawSocketWrite : GWrite {
	Q_OBJECT

public:
	Q_INVOKABLE GRawSocketWrite(QObject* parent = nullptr) : GWrite(parent) {}
	~GRawSocketWrite() override { close(); }

protected:
	bool doOpen() override;
	bool doClose() override;

protected:
	int sd_{0};

public:
	GPacket::Result write(GBuf buf) override;

public slots:
	GPacket::Result write(GPacket* packet) override;

signals:
	void written(GPacket* packet);
};
