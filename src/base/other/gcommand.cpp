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
		QStringList blankArguments;
		int i = QProcess::execute(command, blankArguments);
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
		QStringList blankArguments;
		int i = QProcess::execute(command, blankArguments);
		if (i != EXIT_SUCCESS) {
			QString msg = QString("command(%1) return %2").arg(command).arg(2);
			SET_ERR(GErr::FAIL, msg);
			res = false;
		}
	}
	return res;
}
