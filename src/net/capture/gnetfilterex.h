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

#include "gnetfilter.h"

#ifdef Q_OS_LINUX

// ----------------------------------------------------------------------------
// GNetFilterEx
// ----------------------------------------------------------------------------
struct G_EXPORT GNetFilterEx : GNetFilter {
	Q_OBJECT

public:
	Q_INVOKABLE GNetFilterEx(QObject* parent = nullptr);
	~GNetFilterEx() override;

protected:
	bool doOpen() override;
	bool doClose() override;

public:
	GPacket::Result read(GPacket* packet) override;
	GPacket::Result write(GBuf buf) override;
	GPacket::Result write(GPacket* packet) override;
	GPacket::Result relay(GPacket* packet) override;
	GPacket::Result drop(GPacket* packet) override;

protected:
	void run() override;

	uint32_t id_;
	GIpPacket* ipPacketEx_;
	static int _callbackEx(struct nfq_q_handle* qh, struct nfgenmsg* nfmsg, struct nfq_data* nfad, void* data);
};

#endif
