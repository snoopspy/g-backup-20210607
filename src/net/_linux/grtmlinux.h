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
// GRtmLinux
// ----------------------------------------------------------------------------
struct G_EXPORT GRtmLinux : GRtm {
	friend struct GRtm;
	friend struct GNetInfo;

protected:
	GRtmLinux();
	~GRtmLinux() override;
};
