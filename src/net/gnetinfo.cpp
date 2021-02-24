#include "net/gnetinfo.h"

// ----------------------------------------------------------------------------
// GNetInfo
// ----------------------------------------------------------------------------
GNetInfo::GNetInfo() {
	Q_ASSERT(rtm_.count() == rtm_.intfNames_.count());
	for (int i = 0; i < rtm_.count(); i++) {
		GRtmEntry& entry = const_cast<GRtmEntry&>(rtm_.at(i));
		QString intfName = rtm_.intfNames_.at(i);
		GInterface* netIntf = allInterface_.findByName(intfName);
		if (netIntf == nullptr) {
			QString msg = QString("allInterface_.findByName(%1) return false").arg(intfName);
			qFatal("%s", qPrintable(msg));
		}
		entry.intf_ = netIntf;
	}

	for (GInterface& intf: allInterface_) {
		intf.gateway_ = rtm_.findGateway(intf.name_, intf.ip_);
	}
}

GNetInfo::~GNetInfo() {
}

GNetInfo& GNetInfo::instance() {
	static GNetInfo netInfo;
	return netInfo;
}
