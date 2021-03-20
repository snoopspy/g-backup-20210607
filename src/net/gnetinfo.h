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

#include "net/ginterface.h"
#ifdef Q_OS_LINUX
#include "net/_linux/grtmlinux.h"
#endif // Q_OS_LINUX
#ifdef Q_OS_WIN
#include "net/_win/grtmwin32.h"
#endif // Q_OS_WIN
#include "net/demon/gdemon.h"

// ----------------------------------------------------------------------------
// GNetInfo
// ----------------------------------------------------------------------------
struct G_EXPORT GNetInfo {
protected:
	GNetInfo();
	~GNetInfo() {}


#if defined(Q_OS_LINUX) && !defined(GILGIL_ANDROID_DEBUG) // Q_OS_LINUX
	GLocalInterfaceList interfaceList_;
	GRtmLinux rtm_;
#endif // Q_OS_LINUX
#ifdef Q_OS_WIN
	GLocalInterfaceList interfaceList_;
	GRtmWin32 rtm_;
#endif // Q_OS_WIN
#if !defined(Q_OS_WIN) && defined(GILGIL_ANDROID_DEBUG)
	GRemoteInterfaceList interfaceList_{QString("127.0.0.1"), GDemon::DefaultPort};
	GRemoteRtm rtm_{QString("127.0.0.1"), GDemon::DefaultPort};
#endif // GILGIL_ANDROID_DEBUG

public:
	GInterfaceList& interfaceList() { return interfaceList_; }
	GRtm& rtm() { return rtm_; }

	static GNetInfo& instance() {
		static GNetInfo netInfo;
		return netInfo;
	}
};

// ----------------------------------------------------------------------------
// GRemoteNetInfo
// ----------------------------------------------------------------------------
#include "net/demon/gdemonclient.h"

struct G_EXPORT GRemoteNetInfo {
	friend struct GRemoteNetInfoMap;

protected:
	GRemoteNetInfo(QString ip, quint16 port);
	~GRemoteNetInfo() {}

	GRemoteInterfaceList interfaceList_;
	GRemoteRtm rtm_;

public:
	GRemoteInterfaceList& interfaceList() { return interfaceList_; }
	GRemoteRtm& rtm() { return rtm_; }
	static GRemoteNetInfo& instance(QString ip, quint16 port);
};

// ----------------------------------------------------------------------------
// GRemoteNetInfoMapKey
// ----------------------------------------------------------------------------
struct GRemoteNetInfoMapKey {
	GRemoteNetInfoMapKey(QString ip, uint16_t port) : ip_(ip), port_(port) {}
	QString ip_;
	uint16_t port_;
	bool operator<(const GRemoteNetInfoMapKey &r) const {
		if (ip_ < r.ip_) return true;
		if (ip_ > r.ip_) return false;
		if (port_ < r.port_) return true;
		if (port_ > r.port_) return false;
		return false;
	}
};

// ----------------------------------------------------------------------------
// GRemoteNetInfoMap
// ----------------------------------------------------------------------------
struct GRemoteNetInfoMap : std::map<GRemoteNetInfoMapKey, GRemoteNetInfo*> {
	friend struct GRemoteNetInfo;

protected: // singleton
	virtual ~GRemoteNetInfoMap();
};
