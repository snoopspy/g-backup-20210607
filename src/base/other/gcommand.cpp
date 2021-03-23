#include "gcommand.h"
#include <QProcess>

// ----------------------------------------------------------------------------
// GCommand
// ----------------------------------------------------------------------------
bool GCommand::doOpen() {
	bool res = true;
	for (QString command: openCommands_) {
		if (command.isEmpty()) continue;
		qDebug() << command;
		QStringList arguments = command.split(" ");
		command = arguments.at(0);
		arguments.removeAt(0);
		int i = QProcess::execute(command, arguments);
		if (i != EXIT_SUCCESS) {
			QString msg = QString("command(%1) return %2").arg(command).arg(2);
			SET_ERR(GErr::FAIL, msg);
			res = false;
		}
	}
	return res;
}

bool GCommand::doClose() {
	bool res = true;
	for (QString command: closeCommands_) {
		if (command.isEmpty()) continue;
		qDebug() << command;
		QStringList arguments = command.split(" ");
		command = arguments.at(0);
		arguments.removeAt(0);
		int i = QProcess::execute(command, arguments);
		if (i != EXIT_SUCCESS) {
			QString msg = QString("command(%1) return %2").arg(command).arg(2);
			SET_ERR(GErr::FAIL, msg);
			res = false;
		}
	}
	return res;
}
