#include "grtmlinux.h"
#include "net/ginterface.h"

// ----------------------------------------------------------------------------
// GRtmLinux
// ----------------------------------------------------------------------------

// ip route show table 0 output
//
// [kali linux]
// default via 10.2.2.1 dev eth0 proto dhcp metric 100 (A)
// 10.2.2.0/24 dev eth0 proto kernel scope link src 10.2.2.3 metric 100 (B)
//
// [android]
// default via 10.2.2.1 dev wlan0  table 1021  proto static (C)
// 10.2.2.0/24 dev wlan0  table 1021  proto static  scope link (D)
//

GRtmLinux::GRtmLinux() : GRtm() {
	std::string command("ip route show table 0");
	FILE* p = popen(command.data(), "r");
	if (p == nullptr) {
		qFatal("failed to call %s", command.data());
		return;
	}

	while (true) {
		char buf[256];
		if (std::fgets(buf, 256, p) == nullptr) break;
		GRtmEntry rtmEntry;
		if (checkA(buf, &rtmEntry))
			append(rtmEntry);
		else if (checkB(buf, &rtmEntry))
			append(rtmEntry);
		else if (checkC(buf, &rtmEntry))
			append(rtmEntry);
		else if (checkD(buf, &rtmEntry))
			append(rtmEntry);
	}
}

GRtmLinux::~GRtmLinux() {
	clear();
}

bool GRtmLinux::checkA(char* buf, GRtmEntry* entry) {
	char gateway[256];
	char intf[256];
	int metric;
	// default via 10.2.2.1 dev eth0 proto dhcp metric 100 (A)
	int res = sscanf(buf, "default via %s dev %s proto dhcp metric %d", gateway, intf, &metric);
	if (res == 3) {
		entry->gateway_ = GIp(gateway);
		entry->intfName_ = intf;
		entry->metric_ = metric;
		return true;
	}
	return false;
}

bool GRtmLinux::checkB(char* buf, GRtmEntry* entry) {
	char cidr[256];
	char intf[256];
	char myip[256];
	int metric;
	// 10.2.2.0/24 dev eth0 proto kernel scope link src 10.2.2.3 metric 100 (B)
	int res  = sscanf(buf, "%s dev %s proto kernel scope link src %s metric %d", cidr, intf, myip, &metric);
	if (res == 4) {
		GIp dst;
		GIp mask;
		if (!decodeCidr(cidr, &dst, &mask)) return false;
		entry->dst_ = dst;
		entry->mask_ = mask;
		entry->intfName_ = intf;
		entry->metric_ = metric;
		return true;
	}
	return false;
}

bool GRtmLinux::checkC(char* buf, GRtmEntry* entry) {
	char gateway[256];
	char intf[256];
	// default via 10.2.2.1 dev wlan0  table 1021  proto static (C)
	int res = sscanf(buf, "default via %s dev %s", gateway, intf);
	if (res == 2) {
		entry->gateway_ = GIp(gateway);
		entry->intfName_ = intf;
		return true;
	}
	return false;
}

bool GRtmLinux::checkD(char* buf, GRtmEntry* entry) {
	char cidr[256];
	char intf[256];
	// 10.2.2.0/24 dev wlan0  table 1021  proto static  scope link (D)
	int res = sscanf(buf, "%s dev %s", cidr, intf);
	if (res == 3) {
		GIp dst;
		GIp mask;
		if (!decodeCidr(cidr, &dst, &mask)) return false;
		entry->dst_ = dst;
		entry->mask_ = mask;
		entry->intfName_ = intf;
		return true;
	}
	return false;
}

bool GRtmLinux::decodeCidr(std::string cidr, GIp* dst, GIp* mask) {
	size_t found = cidr.find("/");
	if (found == std::string::npos) return false;
	std::string dstStr = cidr.substr(0, found);
	*dst = GIp(dstStr.data());
	std::string maskStr = cidr.substr(found + 1);
	*mask = numberToMask(std::stoi(maskStr.data()));
	return true;
}

GIp GRtmLinux::numberToMask(int number) {
	uint32_t res = 0;
	for (int i = 0; i < number; i++) {
		res = (res >> 1);
		res |= 0x80000000;
	}
	return res;
}
