// ----------------------------------------------------------------------------
//
// G Library
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

#include "glog.h"

// ----------------------------------------------------------------------------
// GLogManager
// ----------------------------------------------------------------------------
struct G_EXPORT GLogManager : GObjList {
private: // singleton
	GLogManager();
	~GLogManager() override;

public:
	bool enabled_{true};
	static GLogManager& instance();

protected:
	static void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};
