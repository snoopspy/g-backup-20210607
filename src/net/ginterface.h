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

#include <pcap.h>
#include <QList>
#include "gnet.h"
#include "gip.h"
#include "gmac.h"

// ----------------------------------------------------------------------------
// GInterface
// ----------------------------------------------------------------------------
struct G_EXPORT GInterface {
	friend struct GNetInfo;
	friend struct GLocalInterfaceList;
	friend struct GRemoteInterfaceList;

public:
	int index() const { return index_; }
	QString name() const { return name_; }
	QString desc() const { return desc_; }
	GMac mac() const { return mac_; }
	GIp ip() const { return ip_; }
	GIp mask() const { return mask_; }
	GIp gateway() const { return gateway_; }

protected:
	int index_{-1};
	QString name_;
	QString desc_;
	GMac mac_{GMac::nullMac()};
	GIp ip_{0};
	GIp mask_{0};
	GIp gateway_{0};

protected:
	GIp ip_and_mask_{0}; // used for isSameLanIP

public:
	GInterface() {}

public:
	bool isSameLanIp(GIp ip) { return (ip_and_mask_) == (ip & mask_); }
	GIp getAdjIp(GIp ip) { return isSameLanIp(ip) ? ip : gateway_; }
	GIp getStartIp() { return (ip_ & mask_) + 1; }
	GIp getEndIp() { return (ip_ | ~mask_);}

public:
	bool operator==(const GInterface& r) const;
};
// uint qHash(GInterface q); // gilgil temp 2021.03.19

// ----------------------------------------------------------------------------
// GInterfaceList
// ----------------------------------------------------------------------------
struct G_EXPORT GInterfaceList : QList<GInterface> {
protected: // singleton
	GInterfaceList() {}
	virtual ~GInterfaceList() {}

public:
	GInterface* findByName(QString name);
	GInterface* findByIp(GIp ip);
};

// ----------------------------------------------------------------------------
// GLocalInterfaceList
// ----------------------------------------------------------------------------
struct G_EXPORT GLocalInterfaceList : GInterfaceList {
	friend struct GNetInfo;

protected: // singleton
	GLocalInterfaceList();
	~GLocalInterfaceList() override {}
};

// ----------------------------------------------------------------------------
// GRemoteInterfaceList
// ----------------------------------------------------------------------------
struct G_EXPORT GRemoteInterfaceList : GInterfaceList {
	friend struct GNetInfo;
	friend struct GRemoteNetInfo;

protected: // singleton
	GRemoteInterfaceList(QString ip, quint16 port);
	~GRemoteInterfaceList() override {}
};
