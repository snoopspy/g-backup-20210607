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

#include "base/gstateobj.h"
#include "net/packet/gpacket.h"
#include "gflow.h"
#include "gflowkey.h"

// ----------------------------------------------------------------------------
// GFlowMgr
// ----------------------------------------------------------------------------
struct G_EXPORT GFlowMgr : GStateObj {
	Q_OBJECT
	Q_PROPERTY(long checkInterval MEMBER checkInterval_)

public:
	long checkInterval_{1}; // 1 second

public:
	GFlowMgr(QObject* parent = nullptr) : GStateObj(parent) {}
	~GFlowMgr() override { close(); }

protected:
	bool doOpen() override {
		lastCheckTick_ = 0;
		return true;
	}

	bool doClose() override {
		return true;
	}

protected:
	long lastCheckTick_{0};
};
