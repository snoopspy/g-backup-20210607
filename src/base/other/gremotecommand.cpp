#include "gremotecommand.h"

// ----------------------------------------------------------------------------
// GRemoteCommand
// ----------------------------------------------------------------------------
bool GRemoteCommand::doOpen() {
	demonClient_ = new GDemonClient(std::string(qPrintable(ip_)), port_);
	return GCommand::doOpen();
}

bool GRemoteCommand::doClose() {
	bool res = GCommand::doClose();
	if (demonClient_ != nullptr) {
		delete demonClient_;
		demonClient_ = nullptr;
	}
	return res;
}

bool GRemoteCommand::cmdExecute(QString program, QStringList arguments) {
	QString command = program + " " + arguments.join(" ");
	GDemon::CmdExecuteRes res = demonClient_->cmdExecute(qPrintable(command));
	if (!res.result_) {
		SET_ERR(GErr::FAIL, demonClient_->error_.data());
		return false;
	}
	return true;
}

GCommandItem::ProcessId GRemoteCommand::cmdStart(QString program, QStringList arguments) {
	QString command = program + " " + arguments.join(" ");
	GDemon::CmdStartRes res = demonClient_->cmdStart(qPrintable(command));
	if (res.pid_ == 0) {
		SET_ERR(GErr::FAIL, demonClient_->error_.data());
		return 0;
	}
	return res.pid_;
}

bool GRemoteCommand::cmdStop(GCommandItem::ProcessId pid) {
	GDemon::CmdStopRes res = demonClient_->cmdStop(pid);
	if (!res.result_) {
		SET_ERR(GErr::FAIL, demonClient_->error_.data());
		return false;
	}
	return true;
}

bool GRemoteCommand::cmdStartDetached(QString program, QStringList arguments) {
	QString command = program + " " + arguments.join(" ");
	GDemon::CmdStartDetachedRes res = demonClient_->cmdStartDetached(qPrintable(command));
	if (!res.result_) {
		SET_ERR(GErr::FAIL, demonClient_->error_.data());
		return false;
	}
	return true;
}
