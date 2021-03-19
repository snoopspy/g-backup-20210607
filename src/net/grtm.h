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

#include <QList>
#include "ginterface.h"

// ----------------------------------------------------------------------------
// GRtmEntry
// ----------------------------------------------------------------------------
struct G_EXPORT GRtmEntry {
	friend struct GRtm;
	friend struct GRtmLinux;
	friend struct GRemoteRtm;
	friend struct GNetInfo;

#ifdef Q_OS_LINUX
	friend struct GRtmLinux_;
#endif
#ifdef Q_OS_WIN
	friend struct GRtmWin32;
#endif
public:
	GIp dst() const { return dst_; }
	GIp mask() const { return mask_; }
	GIp gateway() const { return gateway_; }
	int metric() const { return metric_; }
	GInterface* intf() const { return intf_; }

protected:
	GIp dst_{0};
	GIp mask_{0};
	GIp gateway_{0};
	int metric_{0};
	GInterface* intf_{nullptr};
	QString intfName_;

public:
	bool operator==(const GRtmEntry& r) const;
};
// uint qHash(GRtmEntry q); // gilgil temp 2021.03.19

// ----------------------------------------------------------------------------
// GRtm(Routing Table Manager)
// ----------------------------------------------------------------------------
struct G_EXPORT GRtm : QList<GRtmEntry> {
	friend struct GRtmEntry;

protected: // inherited singleton
	GRtm();
	virtual ~GRtm();

public:
	GRtmEntry* getBestEntry(GIp ip);
	GIp findGateway(QString intfName, GIp ip);
};

// ----------------------------------------------------------------------------
// GRemoteRtm
// ----------------------------------------------------------------------------
struct G_EXPORT GRemoteRtm : GRtm {
	friend struct GNetInfo;
	friend struct GRemoteNetInfo;

protected: // singleton
	GRemoteRtm(QString ip, quint16 port);
	~GRemoteRtm() override {}
};
