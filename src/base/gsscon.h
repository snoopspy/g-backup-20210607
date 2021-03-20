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

#include "base/gapp.h"

// ----------------------------------------------------------------------------
// GSsCon
// ----------------------------------------------------------------------------
struct G_EXPORT GSsCon : GApp {
	GSsCon(int &argc, char** argv);
	~GSsCon() override;
	bool execFactory(GPluginFactory* pluginFactory = nullptr);

protected:
	GGraph graph_;

public slots:
	void processClose();
	void processSignal(int signo);
};
