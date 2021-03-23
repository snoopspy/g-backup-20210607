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

#include "gcapture.h"
#include "net/packet/gippacket.h"
#include "base/other/gcommand.h"

#ifdef Q_OS_LINUX

// ----------------------------------------------------------------------------
// GNetFilter
// ----------------------------------------------------------------------------
struct G_EXPORT GNetFilter : GCapture {
	Q_OBJECT
	Q_PROPERTY(int queueNum MEMBER queueNum_)
	Q_PROPERTY(int snapLen MEMBER snapLen_)
	Q_PROPERTY(Verdict acceptVerdict MEMBER acceptVerdict_)
	Q_PROPERTY(quint32 mark MEMBER mark_)
	Q_PROPERTY(GObjRef command READ getCommand)
	Q_ENUMS(Verdict)

public:
	enum Verdict {
		ACCEPT = 1,
		REPEAT = 4
	};
	GObjRef getCommand() { return &command_; }

public:
	int queueNum_{0};
	int snapLen_{65536}; // 65536 bytes
	Verdict acceptVerdict_{ACCEPT};
	uint32_t mark_{0};
	GCommand command_;

public:
	Q_INVOKABLE GNetFilter(QObject* parent = nullptr);
	~GNetFilter() override;

protected:
	bool doOpen() override;
	bool doClose() override;

public:
	GPacket::Result read(GPacket* packet) override;
	GPacket::Result write(GPacket* packet) override;
	GPacket::Result write(GBuf buf) override;
	GPacket::Result relay(GPacket* packet) override;

	GPacket::DataLinkType dataLinkType() override { return GPacket::Ip; }
	PathType pathType() override { return InPath; }

protected:
	void run() override;

protected:
	struct nfq_handle* h_{nullptr};
	struct nfq_q_handle* qh_{nullptr};
	int fd_{0};

	GIpPacket ipPacket_{this};

	static int _callback(
			struct nfq_q_handle* qh_,
			struct nfgenmsg* nfmsg,
			struct nfq_data* nfad,
			void* data);
};

#endif
