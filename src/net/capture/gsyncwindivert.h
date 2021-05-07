// ----------------------------------------------------------------------------
//
// G Library
//
// http://gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

#include "gwindivert.h"

#ifdef Q_OS_WIN

// ----------------------------------------------------------------------------
// GSyncWinDivert
// ----------------------------------------------------------------------------
struct G_EXPORT GSyncWinDivert : GWinDivert {
	Q_OBJECT

public:
	Q_INVOKABLE GSyncWinDivert(QObject* parent = nullptr) : GWinDivert(parent) {
		autoRead_ = false;
	}
};

#endif // Q_OS_WIN
