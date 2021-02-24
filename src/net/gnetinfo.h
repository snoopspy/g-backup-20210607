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

#include "net/ginterface.h"
#ifdef Q_OS_LINUX
#include "net/_linux/grtmlinux.h"
#endif // Q_OS_LINUX
#ifdef Q_OS_WIN
#include "net/_win/grtmwin32.h"
#endif // Q_OS_WIN

// ----------------------------------------------------------------------------
// GNetInfo
// ----------------------------------------------------------------------------
struct G_EXPORT GNetInfo {
protected:
	GNetInfo();
	~GNetInfo();

	GAllInterface allInterface_;
#ifdef Q_OS_LINUX
	GRtmLinux rtm_;
#endif // Q_OS_LINUX
#ifdef Q_OS_WIN
	GRtmWin32 rtm_;
#endif // Q_OS_WIN

public:
	GAllInterface& allInterface() { return allInterface_; }
	GRtm& rtm() { return rtm_; }
	static GNetInfo& instance();
};
