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

#include "base/gstateobj.h"
#include <QProcess>

// ----------------------------------------------------------------------------
// GCommandItem
// ----------------------------------------------------------------------------
struct G_EXPORT GCommandItem : public GObj {
	Q_OBJECT
	Q_PROPERTY(QString program MEMBER program_)
	Q_PROPERTY(QStringList arguments MEMBER arguments_)
	Q_PROPERTY(GCommandType commandType MEMBER commandType_)
	Q_ENUMS(GCommandType)

public:
	enum GCommandType {
		Execute,
		Start,
		StartDetach
	};

	QString program_;
	QStringList arguments_;
	GCommandType commandType_{Execute};

	Q_INVOKABLE GCommandItem(QObject* parent = nullptr);
	Q_INVOKABLE GCommandItem(QObject* parent, QString program, QStringList arguments, GCommandType commandType = Execute);
	~GCommandItem() override;

	QProcess* process_{nullptr};
};
typedef GCommandItem* PCommandItem;

// ----------------------------------------------------------------------------
// GCommand
// ----------------------------------------------------------------------------
struct G_EXPORT GCommand : GStateObj {
	Q_OBJECT
	Q_PROPERTY(GObjRefArrayPtr openCommands READ getOpenCommands)
	Q_PROPERTY(GObjRefArrayPtr closeCommands READ getCloseCommands)

public:
	GObjRefArrayPtr getOpenCommands() { return &openCommands_; }
	GObjRefArrayPtr getCloseCommands() { return &closeCommands_; }

public:
	Q_INVOKABLE GCommand(QObject* parent = nullptr) : GStateObj(parent) {}
	~GCommand() override { close(); }

protected:
	bool doOpen() override;
	bool doClose() override;

public:
	GObjRefArray<GCommandItem> openCommands_;
	GObjRefArray<GCommandItem> closeCommands_;
};
