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
					GCommandItem::ProcessId pid = cmdStart(command);
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

std::vector<std::string> GCommand::splitString(std::string s, char ch) {
	std::vector<std::string> res;

	size_t pos = s.find(ch);
	size_t initialPos = 0;
	res.clear();

	while(pos != std::string::npos) {
		res.push_back(s.substr(initialPos, pos - initialPos));
		initialPos = pos + 1;
		pos = s.find(ch, initialPos);
	}
	res.push_back(s.substr(initialPos, std::min(pos, s.size()) - initialPos + 1));

	return res;
}

std::vector<std::string> GCommand::splitCommand(std::string command) {
	std::vector<std::string> split = splitString(command, ' ');
	int index = 1;
	while (true) {
		int count = split.size();
		if (index >= count) break;

		std::string argument = split.at(index);
		char quotation = '\0';
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
				std::string mergeArgument;
				for (int k = index; k <= j; k++) {
					mergeArgument += split.at(k);
					if (k != j)
						mergeArgument += " ";
				}
				mergeArgument = mergeArgument.substr(1, mergeArgument.length() - 2);
				int mergeCount = j - index + 1;
				std::vector<std::string>::iterator eraseIndex = split.begin();
				for (int i = 0; i < index; i++) eraseIndex++;
				for (int k = 0; k < mergeCount; k++) split.erase(eraseIndex);
				split.insert(eraseIndex, mergeArgument);
				index++;
				merged = true;
				break;
			} else
				j++;
		}
		if (!merged) {
			qWarning() << QString("can not find %1 command=%2").arg(quotation), QString(command.data());
			return std::vector<std::string>();
		}
	}

	if (split.size() == 0) {
		qWarning() << QString("split.count is zero command=%1").arg(QString(command.data()));
		return std::vector<std::string>();
	}

	return split;
}

bool GCommand::cmdExecute(QString command) {
	int res = ::system(qPrintable(command));
	if (res != EXIT_SUCCESS) {
		SET_ERR(GErr::FAIL, QString("execute(%1) return %2").arg(command).arg(res));
		return false;
	}
	return true;
}

#include <unistd.h> // for fork
#include <signal.h> // for SIGTERM
GCommandItem::ProcessId GCommand::cmdStart(QString command) {
	std::vector<std::string> arguments = splitCommand(qPrintable(command));
	size_t count = arguments.size();
	if (count == 0) {
		SET_ERR(GErr::FAIL, QString("splitCommand(%1) return nothing").arg(command));
		return 0;
	}

	char *argv[count + 1];
	for (size_t i = 0; i < count; i++)
		argv[i] = pchar(arguments[i].data());
	argv[count] = nullptr;

	pid_t pid = fork();
	if (pid == 0) { // child
		int res = execvp(argv[0], argv);
		qWarning() << QString("not reachable execvp(%1) return %2").arg(command).arg(res);
		exit(EXIT_FAILURE);
	}

	if (pid < 0) { // fail
		QString msg = QString("fork() return %1 errono=%2").arg(pid).arg(std::to_string(errno).data());
		SET_ERR(GErr::FAIL, msg);
		return 0;
	}

	return GCommandItem::ProcessId(pid);
}

#include <wait.h> // for waitpid
bool GCommand::cmdStop(GCommandItem::ProcessId pid) {
	pid_t _pid = pid;
	int killRes = kill(_pid, SIGTERM);

	int state;
	pid_t waitRes = waitpid(_pid, &state, 0);
	qWarning() << QString("kill(%1) return %2 waitpid return %3 state=%4").arg(_pid).arg(killRes).arg(waitRes).arg(state);

	if (killRes != 0) {
		QString msg = QString("kill(%1) rerutn %2 errno=%3").arg(_pid).arg(killRes).arg(std::to_string(errno).data());
		SET_ERR(GErr::FAIL, msg);
		return false;
	}
	return true;
}

bool GCommand::cmdStartDetached(QString command) {
	command += " &";
	int res = ::system(qPrintable(command));
	if (res != EXIT_SUCCESS) {
		SET_ERR(GErr::FAIL, QString("execute(%1) return %2").arg(command).arg(res));
		return false;
	}
	return true;
}
