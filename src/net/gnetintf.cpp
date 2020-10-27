#include <QDebug>
#include "gnetintf.h"
#include "grtm.h"
#ifdef Q_OS_WIN
#include "net/_win/gipadapterinfo.h"
#endif

// ----------------------------------------------------------------------------
// GNetIntf
// ----------------------------------------------------------------------------
GIp GNetIntf::gateway() const {
	GNetIntfs::instance().init();
	return gateway_;
}

GNetIntfs& GNetIntf::all() {
	return GNetIntfs::instance();
}

bool GNetIntf::operator==(const GNetIntf& r) const {
	if (index_ != r.index_) return false;
	if (name_ != r.name_) return false;
	if (desc_ != r.desc_) return false;
	if (dev_ != r.dev_) return false;
	if (mac_ != r.mac_) return false;
	if (ip_ != r.ip_) return false;
	if (mask_ != r.mask_) return false;
	if (gateway_ != r.gateway_) return false;
	return true;
}

uint qHash(GNetIntf q) {
	return uint(uint32_t(q.index()) + q.ip() + q.mask() + q.gateway());
}

// ----------------------------------------------------------------------------
// GNetIntfs
// ----------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#include <net/if.h> // for ifreq
#include <sys/ioctl.h> // for SIOCGIFHWADDR
static GMac getMac(char* name) {
	int s;
	struct ifreq buffer;

	s = socket(PF_INET, SOCK_DGRAM, 0);
	memset(&buffer, 0x00, sizeof(buffer));

	strcpy(buffer.ifr_name, name);
	ioctl(s, SIOCGIFHWADDR, &buffer);
	close(s);

	const u_char* p = const_cast<const u_char*>(reinterpret_cast<u_char*>(buffer.ifr_ifru.ifru_hwaddr.sa_data));
	GMac res = p;
	return res;
}
#endif

GNetIntfs::GNetIntfs() {
	//
	// Initialize allDevs using pcap API.
	//
	if (allDevs_ != nullptr) return;

	char errBuf[PCAP_ERRBUF_SIZE];

	int i = pcap_findalldevs(&allDevs_, errBuf);
	if (i != 0) { // if error occured
		qWarning() << QString("error in pcap_findalldevs_ex (%1)").arg(errBuf);
		return;
	}

	//
	// Add all interfaces
	//
	pcap_if_t* dev = allDevs_;
	i = 1;
	while (dev != nullptr) {
		GNetIntf intf;

		intf.index_ = i;
		intf.name_ = dev->name;
		intf.desc_ = dev->description;
		if (intf.desc_ == "") intf.desc_ = intf.name_;
		intf.dev_ = dev;

#ifdef Q_OS_LINUX
		intf.mac_ = getMac(dev->name);
		for(pcap_addr_t* pa = dev->addresses; pa != nullptr; pa = pa->next) {
			sockaddr* addr = pa->addr;
			sockaddr_in* addr_in = reinterpret_cast<sockaddr_in*>(addr);
			if(addr != nullptr && addr->sa_family == AF_INET)
				intf.ip_ = ntohl(addr_in->sin_addr.s_addr);

			addr = pa->netmask;
			addr_in = reinterpret_cast<sockaddr_in*>(addr);
			if(addr != nullptr && addr->sa_family == AF_INET) {
				intf.mask_ = ntohl(addr_in->sin_addr.s_addr);
			}
		}
#endif
#ifdef Q_OS_WIN
		PIP_ADAPTER_INFO adapter = GIpAdapterInfo::instance().findByAdapterName(dev->name);
		if (adapter != nullptr) {
			intf.desc_ = adapter->Description;
			if (adapter->AddressLength == GMac::SIZE)
				intf.mac_ = adapter->Address;
			intf.ip_ = QString(adapter->IpAddressList.IpAddress.String);
			intf.mask_ = QString(adapter->IpAddressList.IpMask.String);
			intf.gateway_ = QString(adapter->GatewayList.IpAddress.String);
		}
#endif
		intf.ip_and_mask_ = intf.ip_ & intf.mask_;
		push_back(intf);
		dev = dev->next;
		i++;
	}
}

GNetIntfs::~GNetIntfs() {
	clear();

	//
	// Finalize allDevs_
	//
	if (allDevs_ != nullptr) {
		pcap_freealldevs(allDevs_);
		allDevs_ = nullptr;
	}
}

#ifdef Q_OS_LINUX
GNetIntf* GNetIntfs::findByName(QString name) {
	for (GNetIntf& intf: *this) {
		if (intf.name_ == name)
			return &intf;
	}
	return nullptr;
}
#endif
#ifdef Q_OS_WIN
GNetIntf* GNetIntfs::findByName(QString name) {
	for (GNetIntf& intf: *this) {
		if (intf.name_.indexOf(name) != -1)
			return &intf;
	}
	return nullptr;
}
#endif

GNetIntf* GNetIntfs::findByIp(GIp ip) {
	for (GNetIntf& intf: *this) {
		if (intf.ip() == ip)
			return &intf;
	}
	return nullptr;
}

GNetIntfs& GNetIntfs::instance() {
	static GNetIntfs intfs;
	return intfs;
}

void GNetIntfs::init() {
	if (initialized_) return;
	initialized_ = true;

	for (GNetIntf& intf: *this) {
		intf.gateway_ = GRtm::instance().findGateway(intf.name_, intf.ip_);
	}
}
