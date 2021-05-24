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

#include "gpcapwrite.h"
#include "net/gnetinfo.h"

// ----------------------------------------------------------------------------
// GPcapDeviceWrite
// ----------------------------------------------------------------------------
struct G_EXPORT GPcapDeviceWrite : GPcapWrite {
	Q_OBJECT
	Q_PROPERTY(QString intfName MEMBER intfName_)

public:
	QString intfName_{""};

public:
	Q_INVOKABLE GPcapDeviceWrite(QObject* parent = nullptr);
	~GPcapDeviceWrite() override;

protected:
	bool doOpen() override;
	bool doClose() override;

public:
	GInterface* intf() { return intf_; }

protected:
	GInterface* intf_{nullptr};

public:
	GPacket::Result write(GBuf buf) override;

public slots:
	GPacket::Result write(GPacket* packet) override;

signals:
	void written(GPacket* packet);

#ifdef QT_GUI_LIB
public:
	GPropItem* propCreateItem(GPropItemParam* param) override;
#endif // QT_GUI_LIB
};
