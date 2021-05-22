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
			QString program;
			QStringList arguments;
			if (!separate(command, &program, &arguments)) {
				res = false;
				break;
			}
			switch (item->commandType_) {
				case GCommandItem::Execute: {
					int i = QProcess::execute(program, arguments);
					if (i != EXIT_SUCCESS) {
						QString msg = QString("QProcess::execute %1 return %2").arg(command).arg(i);
						SET_ERR(GErr::FAIL, msg);
						res = false;
					}
					break;
				}
				case GCommandItem::StartStop: {
					QProcess* process = new QProcess;
					process->start(program, arguments);
					item->processList_.push_back(process);
					break;
				}
				case GCommandItem::StartDetach: {
					bool ok = QProcess::startDetached(program, arguments);
					if (!ok) {
						QString msg = QString("QProcess::startDetached %1 return false").arg(command);
						SET_ERR(GErr::FAIL, msg);
						res = false;
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
			int index = 0;
			for (QList<QProcess*>::iterator it = item->processList_.begin(); it != item->processList_.end();) {
				QProcess* process = *it;
				process->terminate();
				if (!process->waitForFinished()) {
					process->kill();
					QString msg = QString("waitForFinished %1 return false").arg(item->commands_.at(index));
					SET_ERR(GErr::FAIL, msg);
				}
				delete process;
				it = item->processList_.erase(it);
			}
		}
	}

	for (GObj* obj: closeCommands_) {
		GCommandItem* item = PCommandItem(obj);
		for (QString command: item->commands_) {
			if (command == "") continue;
			qDebug() << command;
			QString program;
			QStringList arguments;
			if (!separate(command, &program, &arguments)) {
				res = false;
				break;
			}
			switch (item->commandType_) {
				case GCommandItem::Execute: {
					int i = QProcess::execute(program, arguments);
					if (i != EXIT_SUCCESS) {
						QString msg = QString("QProcess::execute %1 return %2").arg(command).arg(i);
						SET_ERR(GErr::FAIL, msg);
						res = false;
					}
					break;
				}
				case GCommandItem::StartStop: {
					QString msg = QString("Invalid commandType %1 for '%2'").arg(item->commandType_).arg(command);
					SET_ERR(GErr::FAIL, msg);
					res = false;
					break;
				}
				case GCommandItem::StartDetach: {
					bool ok = QProcess::startDetached(program, arguments);
					if (!ok) {
						QString msg = QString("QProcess::startDetached %1 return false").arg(command);
						SET_ERR(GErr::FAIL, msg);
						res = false;
					}
					break;
				}

			}
			if (!res) break;
		}
	}

	return res;
}

bool GCommand::separate(QString command, QString* program, QStringList* arguments) {
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
			return false;
		}
	}

	if (split.count() == 0) {
		qWarning() << "split.count is zero" << command;
		return false;
	}

	*program = split.at(0);
	split.removeAt(0);
	*arguments = split;
	return true;
}
