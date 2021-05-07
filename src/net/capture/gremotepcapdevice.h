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

#include "gremotepcapcapture.h"

// ----------------------------------------------------------------------------
// GRemotePcapDevice
// ----------------------------------------------------------------------------
struct G_EXPORT GRemotePcapDevice : GRemotePcapCapture {
	Q_OBJECT
	Q_PROPERTY(QString intfName MEMBER intfName_)
	Q_PROPERTY(int snapLen MEMBER snapLen_)
	Q_PROPERTY(int flags MEMBER flags_)
	Q_PROPERTY(int readTimeout MEMBER readTimeout_)
	Q_PROPERTY(int waitTimeout MEMBER waitTimeout_)

public:
	QString intfName_{""};
	int snapLen_{65536}; // 65536 bytes
	int flags_{1}; // PCAP_OPENFLAG_PROMISCUOUS
	int readTimeout_{-1}; // -1 msec
	int waitTimeout_{1}; // 1 msec

public:
	Q_INVOKABLE GRemotePcapDevice(QObject* parent = nullptr);
	~GRemotePcapDevice() override;

protected:
	bool doOpen() override;
	bool doClose() override;

protected:
	GInterface* intf_{nullptr};

#ifdef QT_GUI_LIB
public:
	GPropItem* propCreateItem(GPropItemParam* param) override;
#endif // QT_GUI_LIB
};
