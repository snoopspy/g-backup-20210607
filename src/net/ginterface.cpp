#include <QDebug>
#include "ginterface.h"
#include "grtm.h"
#ifdef Q_OS_WIN
#include "net/_win/gipadapterinfo.h"
#endif

// ----------------------------------------------------------------------------
// GInterface
// ----------------------------------------------------------------------------
bool GInterface::operator==(const GInterface& r) const {
	if (index_ != r.index_) return false;
	if (name_ != r.name_) return false;
	if (desc_ != r.desc_) return false;
	if (mac_ != r.mac_) return false;
	if (ip_ != r.ip_) return false;
	if (mask_ != r.mask_) return false;
	if (gateway_ != r.gateway_) return false;
	return true;
}

uint qHash(GInterface q) {
	return uint(uint32_t(q.index()) + q.ip() + q.mask() + q.gateway());
}

// ----------------------------------------------------------------------------
// GInterfaceList
// ----------------------------------------------------------------------------
#ifdef GILGIL_ANDROID_DEBUG

#include "net/demon/gdemonclient.h"

GInterfaceList::GInterfaceList() {
	GDemonClient& client = GDemonClient::instance();
	for (int i = 0; i < 60 ; i++) { // 60 seconds == 1 minute
		if (client.connect()) break;
		QThread::sleep(1);
	}
	if (client.sd_ == 0) {
		qFatal("can not connect to ssdemon");
	}

	int index = 1;
	GDemon::InterfaceList interfaceList = client.getInterfaceList();
	for (GDemon::Interface& interface: interfaceList) {
		GInterface intf;
		intf.index_ = index;
		intf.name_ = interface.name_.data();
		intf.desc_ = interface.desc_.data();
		intf.mac_ = interface.mac_;
		intf.ip_ = interface.ip_;
		intf.mask_ = interface.mask_;
		intf.ip_and_mask_ = intf.ip_ & intf.mask_;
		// gateway_ is initialized in GNetInfo
		push_back(intf);
		index++;
	}
}

#else // GILGIL_ANDROID_DEBUG

#ifdef Q_OS_LINUX
#include <net/if.h> // for ifreq
#include <sys/ioctl.h> // for SIOCGIFHWADDR
static GMac getMac(char* devName) {
	int s = socket(PF_INET, SOCK_DGRAM, 0);
	if (s == -1) {
		qDebug() << "socket return -1" << strerror(errno);
		return GMac::nullMac();
	}

	struct ifreq buffer;
	memset(&buffer, 0x00, sizeof(buffer));
	strncpy(buffer.ifr_name, devName, IFNAMSIZ);

	int i = ioctl(s, SIOCGIFHWADDR, &buffer);
	close(s);
	if (i  == -1) {
		qDebug() << "ioctl return -1" << devName << strerror(errno);
		return GMac::nullMac();
	}

	const u_char* p = const_cast<const u_char*>(reinterpret_cast<u_char*>(buffer.ifr_ifru.ifru_hwaddr.sa_data));
	GMac res(p);
	return res;
}
#endif // Q_OS_LINUX

GInterfaceList::GInterfaceList() {
	//
	// Initialize allDevs using pcap API.
	//
	pcap_if_t* allDevs;
	char errBuf[PCAP_ERRBUF_SIZE];
	int i = pcap_findalldevs(&allDevs, errBuf);
	if (i != 0) { // if error occured
		qWarning() << QString("error in pcap_findalldevs_ex (%1)").arg(errBuf);
		return;
	}

	//
	// Add all interfaces
	//
	pcap_if_t* dev = allDevs;
	i = 1;
	while (dev != nullptr) {
		GInterface intf;

		intf.index_ = i;
		intf.name_ = dev->name;
		intf.desc_ = dev->description != nullptr ? dev->description : intf.name_;
		if (intf.desc_ == "") intf.desc_ = intf.name_;

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
		// gateway_ is initialized in GNetInfo
		push_back(intf);
		dev = dev->next;
		i++;
	}

	pcap_freealldevs(allDevs);
}

#endif // GILGIL_ANDROID_DEBUG

GInterfaceList::~GInterfaceList() {
	clear();
}

#ifdef Q_OS_LINUX
GInterface* GInterfaceList::findByName(QString name) {
	for (GInterface& intf: *this) {
		if (intf.name_ == name)
			return &intf;
	}
	return nullptr;
}
#endif
#ifdef Q_OS_WIN
GInterface* GInterfaceList::findByName(QString name) {
	for (GInterface& intf: *this) {
		if (intf.name_.indexOf(name) != -1)
			return &intf;
	}
	return nullptr;
}
#endif

GInterface* GInterfaceList::findByIp(GIp ip) {
	for (GInterface& intf: *this) {
		if (intf.ip() == ip)
			return &intf;
	}
	return nullptr;
}
