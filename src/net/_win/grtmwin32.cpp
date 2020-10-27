#include "grtmwin32.h"
#include "net/gnetintf.h"
#include "gipadapterinfo.h"
#include "gipforwardtable.h"

// ----------------------------------------------------------------------------
// GRtmWin32
// ----------------------------------------------------------------------------
GRtmWin32::GRtmWin32() : GRtm() {
	PMIB_IPFORWARDTABLE table = GIpForwardTable::instance().ipForwardTable_;
	for (int i = 0; i < int(table->dwNumEntries); i++) {
		PMIB_IPFORWARDROW row = &table->table[i];
		GRtmEntry entry;
		IF_INDEX ifIndex = row->dwForwardIfIndex;
		PIP_ADAPTER_INFO adapter = GIpAdapterInfo::instance().findByComboIndex(ifIndex);
		if (adapter == nullptr) continue;
		QString adapterName = adapter->AdapterName;
		Q_ASSERT(adapterName != "");
		adapterNames_.push_back(adapterName);
		entry.dst_ = ntohl(row->dwForwardDest);
		entry.gateway_ = ntohl(row->dwForwardNextHop);
		entry.mask_ = ntohl(row->dwForwardMask);
		entry.metric_ = int(row->dwForwardMetric1);

		append(entry);
	}
}

GRtmWin32::~GRtmWin32() {
	clear();
}

void GRtmWin32::init() {
	if (initialized_) return;
	initialized_ = true;

	Q_ASSERT(count() == adapterNames_.count());
	for (int i = 0; i < count(); i++) {
		GRtmEntry& entry = const_cast<GRtmEntry&>(at(i));
		QString adapterName = adapterNames_.at(i);
		GNetIntf* netIntf = GNetIntfs::instance().findByName(adapterName);
		if (netIntf == nullptr) {
			QString msg = QString("GNetIntfs::instance().findByName(%1) return false").arg(adapterName);
			qFatal("%s", qPrintable(msg));
		}
		entry.intf_ = netIntf;
	}
}
