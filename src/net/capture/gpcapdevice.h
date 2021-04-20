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

#include "gpcap.h"
#include "net/ginterface.h"

// ----------------------------------------------------------------------------
// GPcapDevice
// ----------------------------------------------------------------------------
struct G_EXPORT GPcapDevice : GPcap {
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
	Q_INVOKABLE GPcapDevice(QObject* parent = nullptr);
	~GPcapDevice() override;

protected:
	bool doOpen() override;
	bool doClose() override;

public:
	GPacket::Result read(GPacket* packet) override;

protected:
	GInterface* intf_{nullptr};

#ifdef QT_GUI_LIB
public:
	GPropItem* propCreateItem(GPropItemParam* param) override;
#endif // QT_GUI_LIB
};
