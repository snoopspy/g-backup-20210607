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

#include "gpcapwrite.h"
#include "net/gnetintf.h"

// ----------------------------------------------------------------------------
// GPcapDeviceWrite
// ----------------------------------------------------------------------------
struct G_EXPORT GPcapDeviceWrite : GPcapWrite {
	Q_OBJECT
	Q_PROPERTY(QString devName MEMBER devName_)

public:
	QString devName_{""};

public:
	Q_INVOKABLE GPcapDeviceWrite(QObject* parent = nullptr);
	~GPcapDeviceWrite() override;

protected:
	bool doOpen() override;
	bool doClose() override;

public:
	GNetIntf* intf() { return intf_; }
protected:
	GNetIntf* intf_{nullptr};

public:
	GPacket::Result write(GBuf buf);

public slots:
	GPacket::Result write(GPacket* packet);

signals:
	void written(GPacket* packet);

#ifdef QT_GUI_LIB
public:
	GPropItem* propCreateItem(GPropItemParam* param) override;
#endif // QT_GUI_LIB
};
