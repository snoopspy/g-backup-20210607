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
	Q_PROPERTY(QStringList commands MEMBER commands_)
	Q_PROPERTY(GCommandType commandType MEMBER commandType_)
	Q_ENUMS(GCommandType)

public:
	enum GCommandType {
		Execute,
		StartStop,
		StartDetach
	};

	QStringList commands_;
	GCommandType commandType_{Execute};

	Q_INVOKABLE GCommandItem(QObject* parent = nullptr);
	Q_INVOKABLE GCommandItem(QObject* parent, QStringList commands, GCommandType commandType = Execute);
	~GCommandItem() override;

	typedef uint64_t ProcessId;
	QList<ProcessId> processList_;
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

protected:
	static QStringList splitCommand(QString command);

protected:
	virtual bool cmdExecute(QString program, QStringList arguments);
	virtual GCommandItem::ProcessId cmdStart(QString program, QStringList arguments);
	virtual bool cmdStop(GCommandItem::ProcessId pid);
	virtual bool cmdStartDetached(QString program, QStringList arguments);

public:
	GObjRefArray<GCommandItem> openCommands_;
	GObjRefArray<GCommandItem> closeCommands_;
};
