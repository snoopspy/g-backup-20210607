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

// ----------------------------------------------------------------------------
// GRemote
// ----------------------------------------------------------------------------
struct G_EXPORT GRemote : GCapture {
	Q_OBJECT
	Q_PROPERTY(QString ip MEMBER ip_)
	Q_PROPERTY(quint16 port MEMBER port_)

public:
	QString ip_{"127.0.0.1"};
	quint16 port_{8908};

public:
	GRemote(QObject* parent = nullptr) : GCapture(parent) {}
	~GRemote() override {}
};
