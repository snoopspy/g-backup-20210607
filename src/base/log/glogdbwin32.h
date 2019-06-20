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

#ifdef Q_OS_WIN

// ----------------------------------------------------------------------------
// GLogDbWin32
// ----------------------------------------------------------------------------
struct G_EXPORT GLogDbWin32 : GLog {
	void write(QString msg) override;
};

#endif // Q_OS_WIN
