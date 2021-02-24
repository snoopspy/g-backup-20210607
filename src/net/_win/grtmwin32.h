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

#include "net/grtm.h"

// ----------------------------------------------------------------------------
// GRtmWin32
// ----------------------------------------------------------------------------
struct G_EXPORT GRtmWin32 : GRtm {
	friend struct GRtm;
	friend struct GNetInfo;

protected:
	GRtmWin32();
	~GRtmWin32() override;
};
