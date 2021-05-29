#include "gcommand.h"

// ----------------------------------------------------------------------------
// GCommandItem
// ----------------------------------------------------------------------------
GCommandItem::GCommandItem(QObject* parent) : GObj(parent) {
}

GCommandItem::GCommandItem(QObject* parent, QStringList commands, GCommandType commandType) :
	GObj(parent), commands_(commands), commandType_(commandType) {
}

GCommandItem::~GCommandItem() {
	int count = processList_.count();
	if (count != 0) {
		qCritical() << "process list count is " << count;
	}
}

// ----------------------------------------------------------------------------
// GCommand
// ----------------------------------------------------------------------------
bool GCommand::doOpen() {
	bool res = true;

	for (GObj* obj: openCommands_) {
		GCommandItem* item = PCommandItem(obj);
		for (QString command: item->commands_) {
			if (command == "") continue;
			qDebug() << command;
			switch (item->commandType_) {
				case GCommandItem::Execute: {
					res = cmdExecute(command);
					break;
				}
				case GCommandItem::StartStop: {
					pid_t pid = cmdStart(command);
					if (pid != 0)
						item->processList_.push_back(pid);
					else
						res = false;
					break;
				}
				case GCommandItem::StartDetach: {
					res = cmdStartDetached(command);
					break;
				}
			}
			if (!res) break;
		}
	}

	return res;
}

bool GCommand::doClose() {
	bool res = true;

	for (GObj* obj: openCommands_) {
		GCommandItem* item = PCommandItem(obj);
		if (item->commandType_ == GCommandItem::StartStop) {
			for (QList<pid_t>::iterator it = item->processList_.begin(); it != item->processList_.end();) {
				pid_t  pid = *it;
				if (!cmdStop(pid))
					res = false;
				it = item->processList_.erase(it);
			}
		}
	}

	for (GObj* obj: closeCommands_) {
		GCommandItem* item = PCommandItem(obj);
		for (QString command: item->commands_) {
			if (command == "") continue;
			qDebug() << command;
			switch (item->commandType_) {
				case GCommandItem::Execute: {
					res = cmdExecute(command);
					break;
				}
				case GCommandItem::StartStop: {
					QString msg = QString("Invalid commandType %1 for '%2'").arg(item->commandType_).arg(command);
					SET_ERR(GErr::FAIL, msg);
					res = false;
					break;
				}
				case GCommandItem::StartDetach:{
					res = cmdStartDetached(command);
					break;
				}
			}
			if (!res) break;
		}
	}

	return res;
}

bool GCommand::cmdExecute(QString command) {
	std::string error;
	bool res = GProcess::execute(qPrintable(command), error);
	if (!res)
		SET_ERR(GErr::FAIL, QString(error.data()));
	return res;
}


pid_t GCommand::cmdStart(QString command) {
	std::string error;
	pid_t pid = GProcess::execute(qPrintable(command), error);
	if (pid == 0)
		SET_ERR(GErr::FAIL, QString(error.data()));
	return pid;
}

bool GCommand::cmdStop(pid_t pid) {
	std::string error;
	bool res = GProcess::stop(pid, error);
	if (!res)
		SET_ERR(GErr::FAIL, QString(error.data()));
	return res;
}

bool GCommand::cmdStartDetached(QString command) {
	std::string error;
	bool res = GProcess::startDetached(qPrintable(command), error);
	if (!res)
		SET_ERR(GErr::FAIL, QString(error.data()));
	return res;
}
