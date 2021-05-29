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

#include "gcommand.h"
#include "net/demon/gdemonclient.h"

// ----------------------------------------------------------------------------
// GRemoteCommand
// ----------------------------------------------------------------------------
struct G_EXPORT GRemoteCommand : GCommand {
	Q_OBJECT
	Q_PROPERTY(QString ip MEMBER ip_)
	Q_PROPERTY(quint16 port MEMBER port_)

public:
	QString ip_{"127.0.0.1"};
	quint16 port_{GDemon::DefaultPort};

public:
	Q_INVOKABLE GRemoteCommand(QObject* parent = nullptr) : GCommand(parent) {}
	~GRemoteCommand() override { close(); }

protected:
	bool doOpen() override;
	bool doClose() override;

protected:
	GDemonClient* demonClient_{nullptr};

	bool cmdExecute(QString command) override;
	pid_t cmdStart(QString command) override;
	bool cmdStop(pid_t pid) override;
	bool cmdStartDetached(QString command) override;
};
