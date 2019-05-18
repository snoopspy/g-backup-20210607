#include "grtmwin32.h"
#include "net/gnetintf.h"
#include "gipadapterinfo.h"
#include "gipforwardtable.h"

// ----------------------------------------------------------------------------
// GRtmWin32
// ----------------------------------------------------------------------------
bool GRtmWin32::init() {
  if (initialized_) return false;
  initialized_ = true;
  clear();

  PMIB_IPFORWARDTABLE table = GIpForwardTable::instance().ipForwardTable_;
  for (int i = 0; i < int(table->dwNumEntries); i++) {
    PMIB_IPFORWARDROW row = &table->table[i];
    GRtmEntry entry;
    IF_INDEX ifIndex = row->dwForwardIfIndex;
    PIP_ADAPTER_INFO adapter = GIpAdapterInfo::instance().findByComboIndex(ifIndex);
    if (adapter == nullptr) continue;
    QString adapterName = adapter->AdapterName;
    GNetIntf* netIntf = GNetIntfs::instance().findByName(adapterName);
    if (netIntf == nullptr) {
      qCritical() << QString("GNetIntfs::instance().findByName(%1) return nullptr").arg(adapterName);
      continue;
    }
    entry.intf_ = netIntf;
    entry.dst_ = ntohl(row->dwForwardDest);
    entry.gateway_ = ntohl(row->dwForwardNextHop);
    entry.mask_ = ntohl(row->dwForwardMask);
    entry.metric_ = int(row->dwForwardMetric1);

    append(entry);
  }
  return true;
};
