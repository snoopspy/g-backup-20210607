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

// ----------------------------------------------------------------------------
// GCommand
// ----------------------------------------------------------------------------
struct G_EXPORT GCommand : GStateObj {
	Q_OBJECT
	Q_PROPERTY(QStringList openCommands MEMBER openCommands_)
	Q_PROPERTY(QStringList closeCommands MEMBER closeCommands_)

public:
	Q_INVOKABLE GCommand(QObject* parent = nullptr) : GStateObj(parent) {}
	~GCommand() override { close(); }

protected:
	bool doOpen() override;
	bool doClose() override;

public:
	QStringList openCommands_;
	QStringList closeCommands_;
};
