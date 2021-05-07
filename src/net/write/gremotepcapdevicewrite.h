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

#include "gremotepcapwrite.h"

// ----------------------------------------------------------------------------
// GRemotePcapDeviceWrite
// ----------------------------------------------------------------------------
struct G_EXPORT GRemotePcapDeviceWrite : GRemotePcapWrite {
	Q_OBJECT
	Q_PROPERTY(QString intfName MEMBER intfName_)

public:
	QString intfName_{""};

public:
	Q_INVOKABLE GRemotePcapDeviceWrite(QObject* parent = nullptr);
	~GRemotePcapDeviceWrite() override;

protected:
	bool doOpen() override;
	bool doClose() override;

public:
	GInterface* intf() { return intf_; }
protected:
	GInterface* intf_{nullptr};

#ifdef QT_GUI_LIB
public:
	GPropItem* propCreateItem(GPropItemParam* param) override;
#endif // QT_GUI_LIB
};
