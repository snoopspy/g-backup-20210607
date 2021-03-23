#include "gcommand.h"

// ----------------------------------------------------------------------------
// GCommandItem
// ----------------------------------------------------------------------------
GCommandItem::GCommandItem(QObject* parent) : GObj(parent) {
}

GCommandItem::GCommandItem(QObject* parent, QString program, QStringList arguments, GCommandType commandType) :
	GObj(parent), program_(program), arguments_(arguments), commandType_(commandType) {
}

GCommandItem::~GCommandItem() {
	if (process_ != nullptr) {
		delete process_;
		process_ = nullptr;
	}
}

// ----------------------------------------------------------------------------
// GCommand
// ----------------------------------------------------------------------------
bool GCommand::doOpen() {
	bool res = true;

	for (GObj* obj: openCommands_) {
		GCommandItem* item = PCommandItem(obj);
		QString all = item->program_ + " " + item->arguments_.join(" ");
		qDebug() << all;
		switch (item->commandType_) {
			case GCommandItem::Execute: {
				int i = QProcess::execute(item->program_, item->arguments_);
				if (i != EXIT_SUCCESS) {
					QString msg = QString("QProcess::execute %1 return %2").arg(all).arg(i);
					SET_ERR(GErr::FAIL, msg);
					res = false;
				}
				break;
			}
			case GCommandItem::Start: {
				item->process_ = new QProcess;
				item->process_->start(item->program_, item->arguments_);
				break;
			}
			case GCommandItem::StartDetach: {
				bool ok = QProcess::startDetached(item->program_, item->arguments_);
				if (!ok) {
					QString msg = QString("QProcess::startDetached %1 return false").arg(all);
					SET_ERR(GErr::FAIL, msg);
					res = false;
				}
				break;
			}
		}
		if (!res) break;
	}

	return res;
}

bool GCommand::doClose() {
	bool res = true;

	for (GObj* obj: openCommands_) {
		GCommandItem* item = PCommandItem(obj);
		if (item->commandType_ == GCommandItem::Start) {
			if (!item->process_->waitForFinished()) {
				QString all = item->program_ + " " + item->arguments_.join(" ");
				QString msg = QString("waitForFinished %1 return false").arg(all);
				SET_ERR(GErr::FAIL, msg);
				res = false;
			}
			delete item->process_;
			item->process_ = nullptr;
		}
	}

	for (GObj* obj: closeCommands_) {
		GCommandItem* item = PCommandItem(obj);
		QString all = item->program_ + " " + item->arguments_.join(" ");
		qDebug() << all;
		switch (item->commandType_) {
			case GCommandItem::Execute: {
				int i = QProcess::execute(item->program_, item->arguments_);
				if (i != EXIT_SUCCESS) {
					QString msg = QString("QProcess::execute %1 return %2").arg(all).arg(i);
					SET_ERR(GErr::FAIL, msg);
					res = false;
				}
				break;
			}
			case GCommandItem::Start: {
				QString msg = QString("Invalid commandType %1 for '%2'").arg(item->commandType_).arg(all);
				SET_ERR(GErr::FAIL, msg);
				res = false;
				break;
			}
			case GCommandItem::StartDetach: {
				bool ok = QProcess::startDetached(item->program_, item->arguments_);
				if (!ok) {
					QString msg = QString("QProcess::startDetached %1 return false").arg(all);
					SET_ERR(GErr::FAIL, msg);
					res = false;
				}
				break;
			}
		}
		if (!res) break;
	}

	return res;
}
