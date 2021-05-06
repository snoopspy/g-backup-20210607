#include "net/gnetinfo.h"

// ----------------------------------------------------------------------------
// GNetInfo
// ----------------------------------------------------------------------------
GNetInfo::GNetInfo() {
	for (int i = 0; i < rtm_.count(); i++) {
		GRtmEntry& entry = const_cast<GRtmEntry&>(rtm_.at(i));
		QString intfName = entry.intfName_;
		GInterface* intf = interfaceList_.findByName(intfName);
		if (intf == nullptr) {
			QString msg = QString("interfaceList_.findByName(%1) return false").arg(intfName);
			qCritical() << msg;
		}
		entry.intf_ = intf;
	}

	for (GInterface& intf: interfaceList_) {
		intf.gateway_ = rtm_.findGateway(intf.name_, intf.ip_);
	}
}

// ----------------------------------------------------------------------------
// GRemoteNetInfo
// ----------------------------------------------------------------------------
GRemoteNetInfo::GRemoteNetInfo(QString ip, quint16 port) : interfaceList_(ip, port), rtm_(ip, port) {
	for (int i = 0; i < rtm_.count(); i++) {
		GRtmEntry& entry = const_cast<GRtmEntry&>(rtm_.at(i));
		QString intfName = entry.intfName_;
		GInterface* intf = interfaceList_.findByName(intfName);
		if (intf == nullptr) {
			QString msg = QString("interfaceList_.findByName(%1) return false").arg(intfName);
			qCritical() << msg;
		}
		entry.intf_ = intf;
	}

	for (GInterface& intf: interfaceList_) {
		intf.gateway_ = rtm_.findGateway(intf.name_, intf.ip_);
	}
}

GRemoteNetInfo& GRemoteNetInfo::instance(QString ip, quint16 port) {
	static GRemoteNetInfoMap map;
	GRemoteNetInfoMapKey key(ip, port);
	GRemoteNetInfoMap::iterator it = map.find(key);
	if (it == map.end()) {
		map.insert(key, new GRemoteNetInfo(ip, port));
		it = map.find(key);
	}
	return *it.value();
}

// ----------------------------------------------------------------------------
// GRemoteNetInfoMap
// ----------------------------------------------------------------------------
GRemoteNetInfoMap::~GRemoteNetInfoMap() {
	GRemoteNetInfoMap::iterator it = begin();
	while (it != end()) {
		delete *it;
		it = erase(it);
	}
};
