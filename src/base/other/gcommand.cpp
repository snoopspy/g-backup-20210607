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
			QStringList arguments = splitCommand(command);
			if (arguments.count() == 0) {
				SET_ERR(GErr::FAIL, QString("splitCommand(%1) return nothing").arg(command));
				res = false;
				break;
			}
			QString program = arguments.at(0);
			arguments.removeAt(0);
			switch (item->commandType_) {
				case GCommandItem::Execute: {
					res = cmdExecute(program, arguments);
					break;
				}
				case GCommandItem::StartStop: {
					GCommandItem::ProcessId pid = cmdStart(program, arguments);
					if (pid != 0)
						item->processList_.push_back(pid);
					else
						res = false;
					break;
				}
				case GCommandItem::StartDetach: {
					res = cmdStartDetached(program, arguments);
					if (res) {
						QString msg = QString("QProcess::startDetached %1 return false").arg(command);
						SET_ERR(GErr::FAIL, msg);
					}
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
			for (QList<GCommandItem::ProcessId>::iterator it = item->processList_.begin(); it != item->processList_.end();) {
				GCommandItem::ProcessId pid = *it;
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
			QStringList arguments = splitCommand(command);
			if (arguments.count() == 0) {
				SET_ERR(GErr::FAIL, QString("splitCommand(%1) return nothing").arg(command));
				res = false;
				break;
			}
			QString program = arguments.at(0);
			arguments.removeAt(0);
			switch (item->commandType_) {
				case GCommandItem::Execute: {
					res = cmdExecute(program, arguments);
					break;
				}
				case GCommandItem::StartStop: {
					QString msg = QString("Invalid commandType %1 for '%2'").arg(item->commandType_).arg(command);
					SET_ERR(GErr::FAIL, msg);
					res = false;
					break;
				}
				case GCommandItem::StartDetach:{
					res = cmdStartDetached(program, arguments);
					if (res) {
						QString msg = QString("QProcess::startDetached %1 return false").arg(command);
						SET_ERR(GErr::FAIL, msg);
					}
					break;
				}
			}
			if (!res) break;
		}
	}

	return res;
}

QStringList GCommand::splitCommand(QString command) {
	QStringList split = command.split(' ');
	int index = 1;
	while (true) {
		int count = split.count();
		if (index >= count) break;

		QString argument = split.at(index);
		QChar quotation = '\0';
		if (argument.at(0) == '\'') quotation = '\'';
		else if (argument.at(0) == '\"') quotation = '\"';
		if (quotation == '\0') {
			index++;
			continue;
		}

		int j = index + 1;
		bool merged = false;
		while (j < count) {
			argument = split.at(j);
			if (argument.at(argument.length() - 1) == quotation) {
				QString mergeArgument;
				for (int k = index; k <= j; k++) {
					mergeArgument += split.at(k);
					if (k != j)
						mergeArgument += " ";
				}
				mergeArgument = mergeArgument.mid(1, mergeArgument.length() - 2);
				int mergeCount = j - index + 1;
				for (int k = 0; k < mergeCount; k++) split.removeAt(index);
				split.insert(index, mergeArgument);
				index++;
				merged = true;
				break;
			} else
				j++;
		}
		if (!merged) {
			qWarning() << "can not find " << quotation;
			return QStringList();
		}
	}

	if (split.count() == 0) {
		qWarning() << "split.count is zero" << command;
		return QStringList();
	}

	return split;
}

bool GCommand::cmdExecute(QString program, QStringList arguments) {
	int res = QProcess::execute(program, arguments);
	if (res != EXIT_SUCCESS) {
		SET_ERR(GErr::FAIL, QString("execute(%1) return %2").arg(program + " " + arguments.join(" ")).arg(res));
		return false;
	}
	return true;
}

GCommandItem::ProcessId GCommand::cmdStart(QString program, QStringList arguments) {
	QProcess* process = new QProcess;
	process->start(program, arguments);
	GCommandItem::ProcessId pid = GCommandItem::ProcessId(process);
	if (pid == 0) {
		SET_ERR(GErr::FAIL, QString("can not execute %1").arg(program + " " + arguments.join(" ")));
		return false;
	}
	return pid;
}

bool GCommand::cmdStop(GCommandItem::ProcessId pid) {
	QProcess* process = reinterpret_cast<QProcess*>(pid);
	Q_ASSERT(process != nullptr);
	process->terminate();
	bool res = process->waitForFinished();
	if (!res) {
		SET_ERR(GErr::FAIL, QString("waitForFinished(%1) return false").arg(QString::number(pid, 16)));
		process->kill();
	}
	delete process;
	return res;
}

bool GCommand::cmdStartDetached(QString program, QStringList arguments) {
	bool res = QProcess::startDetached(program, arguments);
	if (!res) {
		SET_ERR(GErr::FAIL, QString("can not execute %1").arg(program + " " + arguments.join(" ")));
		return false;
	}
	return true;
}
