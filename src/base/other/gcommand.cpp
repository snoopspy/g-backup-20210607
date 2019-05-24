#include "gcommand.h"
#include <QProcess>

// ----------------------------------------------------------------------------
// GCommand
// ----------------------------------------------------------------------------
GCommand::GCommand(QObject* parent) : GStateObj(parent) {
}

GCommand::~GCommand() {
  close();
}

bool GCommand::doOpen() {
  bool res = true;
  for (QString command: openCommandList_) {
    if (command.isEmpty()) continue;
    int res = QProcess::execute(command);
    if (res != EXIT_SUCCESS) {
      qWarning() << QString("command(%1) return %2").arg(command).arg(2);
      res = false;
    }
  }
  return res;
}

bool GCommand::doClose() {
  bool res = true;
  for (QString command: closeCommandList_) {
    if (command.isEmpty()) continue;
    int res = QProcess::execute(command);
    if (res != EXIT_SUCCESS) {
      qWarning() << QString("command(%1) return %2").arg(command).arg(2);
      res = false;
    }
  }
  return res;
}
