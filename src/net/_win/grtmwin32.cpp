#include "grtmwin32.h"
#include "net/ginterface.h"
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
		entry.intfName_ = adapterName;
		entry.dst_ = ntohl(row->dwForwardDest);
		entry.gateway_ = ntohl(row->dwForwardNextHop);
		entry.mask_ = ntohl(row->dwForwardMask);
		entry.metric_ = int(row->dwForwardMetric1);
		// intf_ is initialized in GNetInfo
		append(entry);
	}
}

GRtmWin32::~GRtmWin32() {
	clear();
}
