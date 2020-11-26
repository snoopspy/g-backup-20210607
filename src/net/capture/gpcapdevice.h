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

#include "gpcap.h"
#include "net/gnetintf.h"

// ----------------------------------------------------------------------------
// GPcapDevice
// ----------------------------------------------------------------------------
struct G_EXPORT GPcapDevice : GPcap {
	Q_OBJECT
	Q_PROPERTY(QString devName MEMBER devName_)
	Q_PROPERTY(int snapLen MEMBER snapLen_)
	Q_PROPERTY(int flags MEMBER flags_)
	Q_PROPERTY(int timeout MEMBER timeout_)
	Q_PROPERTY(int waitTimeout MEMBER waitTimeout_)

public:
	QString devName_{""};
	int snapLen_{65536}; // 65536 bytes
	int flags_{1}; // PCAP_OPENFLAG_PROMISCUOUS
#ifdef Q_OS_LINUX
	int timeout_{-1}; // 1 msec
	int waitTimeout_{1}; // 1 msec
#else // Q_OSWIN
	int timeout_{1}; // 1 msec
	int waitTimeout_{0}; // 1 msec
#endif // Q_OS_LINUX

public:
	Q_INVOKABLE GPcapDevice(QObject* parent = nullptr);
	~GPcapDevice() override;

protected:
	bool doOpen() override;
	bool doClose() override;

public:
	GPacket::Result read(GPacket* packet) override;

public:
	GNetIntf* intf() { return intf_; }
protected:
	GNetIntf* intf_{nullptr};

#ifdef QT_GUI_LIB
public:
	GPropItem* propCreateItem(GPropItemParam* param) override;
#endif // QT_GUI_LIB
};
