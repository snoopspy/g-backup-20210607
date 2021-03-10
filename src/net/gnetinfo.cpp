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
			qFatal("%s", qPrintable(msg));
		}
		entry.intf_ = intf;
	}

	for (GInterface& intf: interfaceList_) {
		intf.gateway_ = rtm_.findGateway(intf.name_, intf.ip_);
	}
}

GNetInfo::~GNetInfo() {
}

GNetInfo& GNetInfo::instance() {
	static GNetInfo netInfo;
	return netInfo;
}
