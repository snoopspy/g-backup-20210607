// ----------------------------------------------------------------------------
//
// G Library
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

#include "base/gstateobj.h"
#include "base/sys/gthread.h"
#include "net/packet/gpacket.h"

// ----------------------------------------------------------------------------
// GCaptureThread
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// GCapture
// ----------------------------------------------------------------------------
struct G_EXPORT GCapture : GStateObj {
	Q_OBJECT
	Q_PROPERTY(bool enabled MEMBER enabled_)
	Q_PROPERTY(bool autoParse MEMBER autoParse_)
	// Q_PROPERTY(GParser*) // gilgil temp 2019.05.14

public:
	bool enabled_{true};
	bool autoParse_{true};

protected:
	bool autoRead_{true};

	typedef enum {
		InPath,
		OutOfPath
	} PathType;

public:
	GCapture(QObject* parent = nullptr) : GStateObj(parent) {}
	~GCapture() override;

protected:
	bool captureThreadOpen();
	bool captureThreadClose();

public:
	virtual GPacket::Result read(GPacket* packet);
	virtual GPacket::Result write(GPacket* packet);
	virtual GPacket::Result write(GBuf buf);
	virtual GPacket::Result relay(GPacket* packet);

	virtual GPacket::DataLinkType dataLinkType() { return GPacket::Null; }
	virtual PathType pathType() { return OutOfPath; }

protected:
	struct Thread : GThread {
		Thread(GCapture* capture) : GThread(capture) {}
	protected:
		void run() override {
			GCapture* capture = dynamic_cast<GCapture*>(parent());
			Q_ASSERT(capture != nullptr);
			capture->run();
		}
	} thread_{this};

	virtual void run();

signals:
	void captured(GPacket* packet);
};
