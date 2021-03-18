#include "gdemonserver.h"
#include "gtrace.h"

// ----------------------------------------------------------------------------
// GDemonServer
// ----------------------------------------------------------------------------
GDemonServer::GDemonServer() {
}

GDemonServer::~GDemonServer() {
	stop();
	wait();
}

bool GDemonServer::start(uint16_t port) {
	accept_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if (accept_ == -1) {
		GTRACE("%s", strerror(errno));
		return false;
	}

	int optval = 1;
	int res = ::setsockopt(accept_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	if (res == -1) {
		GTRACE("%s", strerror(errno));
		return false;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	res = ::bind(accept_, (struct sockaddr *)&addr, sizeof(addr));
	if (res == -1) {
		GTRACE("%s", strerror(errno));
		return false;
	}

	res = listen(accept_, 5);
	if (res == -1) {
		GTRACE("%s", strerror(errno));
		return false;
	}

	return true;
}

void GDemonServer::exec() {
	while (true) {
		struct sockaddr_in cli_addr;
		socklen_t len = sizeof(cli_addr);
		int new_sd = accept(accept_, (struct sockaddr *)&cli_addr, &len);
		if (new_sd == -1) {
			GTRACE("%s", strerror(errno));
			break;
		}
		new std::thread(Session::_run, this, new_sd);
	}
}

void GDemonServer::stop() {
	if (accept_ != 0) {
		close(accept_);
		accept_ = 0;
	}

	sessions_.lock();
	for (Session* session: sessions_) {
		::close(session->sd_);
	}
	sessions_.unlock();
}

void GDemonServer::wait() {
	while(true) {
		sessions_.lock();
		bool exit = sessions_.size() == 0;
		sessions_.unlock();
		if (exit) break;
		sleep(1);
	}
}

void GDemonServer::Session::_run(GDemonServer* owner, int new_sd) {
	Session* session = new Session;
	session->sd_ = new_sd;

	owner->sessions_.lock();
	owner->sessions_.push_back(session);
	owner->sessions_.unlock();

	session->run();

	owner->sessions_.lock();
	owner->sessions_.remove(session);
	owner->sessions_.unlock();
}

void GDemonServer::Session::run() {
	GTRACE("beg");

	bool active = true;
	while (active) {
		char buffer[MaxBufferSize];
		Header* header = GDemon::PHeader(buffer);
		if (!recvAll(sd_, header, sizeof(Header)))
			break;
		if (header->len_ < 0 || header->len_ > MaxBufferSize) {
			GTRACE("invalid len_ %d", header->len_);
			break;
		}

		if (!recvAll(sd_, buffer + sizeof(Header), header->len_))
			break;

		pchar buf = buffer;
		int size = header->len_;
		switch (header->cmd_) {
			case cmdGetInterfaceList:
				active = processGetInterfaceList(buf, size);
				break;
			case cmdGetRtm:
				active = processGetRtm(buf, size);
				break;
			case cmdPcapOpen:
				active = processPcapOpen(buf, size);
				break;
			case cmdPcapClose:
				active = processPcapClose(buf, size);
				break;
			default:
				GTRACE("invalid cmd %d", header->cmd_);
				active = false;
				break;
		}
	}

	::close(sd_);
	if (pcap_ != nullptr)
		pcap_close(pcap_);

	GTRACE("end");
}

#include <net/if.h> // for ifreq
#include <sys/ioctl.h> // for SIOCGIFHWADDR
bool getMac(char* devName, uint8_t* mac) {
	int s = socket(PF_INET, SOCK_DGRAM, 0);
	if (s == -1) {
		GTRACE("socket return -1 %s", strerror(errno));
		return false;
	}

	struct ifreq buffer;
	memset(&buffer, 0x00, sizeof(buffer));
	strncpy(buffer.ifr_name, devName, IFNAMSIZ);

	ioctl(s, SIOCGIFHWADDR, &buffer);
	close(s);

	memcpy(mac, buffer.ifr_ifru.ifru_hwaddr.sa_data, GDemon::Interface::MacSize);
	return true;
}

bool GDemonServer::Session::processGetInterfaceList(pchar, int32_t) {
	GTRACE("");

	pcap_if_t* allDevs;
	char errBuf[PCAP_ERRBUF_SIZE];
	// GTRACE("bef call pcap_findalldevs"); // gilgil temp 2021.03.11
	int i = pcap_findalldevs(&allDevs, errBuf);
	// GTRACE("aft call pcap_findalldevs i=%d", i); // gilgil temp 2021.03.11
	if (i != 0) { // if error occured
		GTRACE("error in pcap_findalldevs_ex (%s)", errBuf);
		return false;
	}

	//
	// Add all interfaces
	//
	pcap_if_t* dev = allDevs;
	GetInterfaceListRep rep;
	i = 1;
	while (dev != nullptr) {
		Interface interface;

		interface.index_ = i;
		interface.name_ = dev->name;
		interface.desc_ = dev->description != nullptr ? dev->description : interface.name_;
		getMac(dev->name, interface.mac_);
		for(pcap_addr_t* pa = dev->addresses; pa != nullptr; pa = pa->next) {
			sockaddr* addr = pa->addr;
			sockaddr_in* addr_in = reinterpret_cast<sockaddr_in*>(addr);
			if(addr != nullptr && addr->sa_family == AF_INET)
				interface.ip_ = ntohl(addr_in->sin_addr.s_addr);

			addr = pa->netmask;
			addr_in = reinterpret_cast<sockaddr_in*>(addr);
			if(addr != nullptr && addr->sa_family == AF_INET) {
				interface.mask_ = ntohl(addr_in->sin_addr.s_addr);
			}
		}
		rep.interfaceList_.push_back(interface);
		dev = dev->next;
		i++;
	}
	pcap_freealldevs(allDevs);

	char buffer[MaxBufferSize];
	int32_t encLen = rep.encode(buffer, MaxBufferSize);
	if (encLen == -1) {
		GTRACE("rep.encode return -1");
		return false;
	}

	int sendLen = ::send(sd_, buffer, encLen, 0);
	if (sendLen == 0 || sendLen == -1) {
		GTRACE("send return %d", sendLen);
		return false;
	}
	return true;
}

//
// ip route show table 0 output
//
// [kali linux]
// default via 10.2.2.1 dev eth0 proto dhcp metric 100 (A)
// 10.2.2.0/24 dev eth0 proto kernel scope link src 10.2.2.3 metric 100 (B)
//
// [android]
// default via 10.2.2.1 dev wlan0  table 1021  proto static (C)
// 10.2.2.0/24 dev wlan0  proto kernel  scope link  src 10.2.2.189 (D)
//
bool GDemonServer::Session::processGetRtm(pchar buf, int32_t size) {
	GTRACE("");

	std::string command("ip route show table 0");
	FILE* p = popen(command.data(), "r");
	if (p == nullptr) {
		GTRACE("failed to call %s", command.data());
		return false;
	}

	GetRtmRep rep;
	while (true) {
		char buf[256];
		if (std::fgets(buf, 256, p) == nullptr) break;
		RtmEntry entry;
		if (RtmFunc::checkA(buf, &entry))
			rep.rtm_.push_back(entry);
		else if (RtmFunc::checkB(buf, &entry))
			rep.rtm_.push_back(entry);
		else if (RtmFunc::checkC(buf, &entry))
			rep.rtm_.push_back(entry);
		else if (RtmFunc::checkD(buf, &entry))
			rep.rtm_.push_back(entry);
	}

	char buffer[MaxBufferSize];
	int32_t encLen = rep.encode(buffer, MaxBufferSize);
	if (encLen == -1) {
		GTRACE("rep.encode return -1");
		return false;
	}

	int sendLen = ::send(sd_, buffer, encLen, 0);
	if (sendLen == 0 || sendLen == -1) {
		GTRACE("send return %d", sendLen);
		return false;
	}
	return true;
}

bool GDemonServer::Session::processPcapOpen(pchar buf, int32_t size) {
	GTRACE("processPcapOpen");
	(void)buf; (void)size;
	return false;
}

bool GDemonServer::Session::processPcapClose(pchar buf, int32_t size) {
	GTRACE("processPcapClose");
	(void)buf; (void)size;
	return false;
}

bool GDemonServer::RtmFunc::checkA(char* buf, RtmEntry* entry) {
	char gateway[256];
	char intf[256];
	int metric;
	// default via 10.2.2.1 dev eth0 proto dhcp metric 100 (A)
	int res = sscanf(buf, "default via %s dev %s proto dhcp metric %d", gateway, intf, &metric);
	if (res == 3) {
		GTRACE("checkA %s", buf); // gilgil temp
		struct in_addr addr;
		inet_aton(gateway, &addr);
		entry->gateway_ = ntohl(addr.s_addr);
		entry->intfName_ = intf;
		entry->metric_ = metric;
		return true;
	}
	return false;
}

bool GDemonServer::RtmFunc::checkB(char* buf, RtmEntry* entry) {
	char cidr[256];
	char intf[256];
	char myip[256];
	int metric;
	// 10.2.2.0/24 dev eth0 proto kernel scope link src 10.2.2.3 metric 100 (B)
	int res  = sscanf(buf, "%s dev %s proto kernel scope link src %s metric %d", cidr, intf, myip, &metric);
	if (res == 4) {
		GTRACE("checkB %s", buf); // gilgil temp
		uint32_t dst;
		uint32_t mask;
		if (!decodeCidr(cidr, &dst, &mask)) return false;
		entry->dst_ = dst;
		entry->mask_ = mask;
		entry->intfName_ = intf;
		entry->metric_ = metric;
		return true;
	}
	return false;
}

bool GDemonServer::RtmFunc::checkC(char* buf, RtmEntry* entry) {
	char gateway[256];
	char intf[256];
	// default via 10.2.2.1 dev wlan0  table 1021  proto static (C)
	int res = sscanf(buf, "default via %s dev %s", gateway, intf);
	if (res == 2) {
		GTRACE("checkC %s", buf); // gilgil temp
		struct in_addr addr;
		inet_aton(gateway, &addr);
		entry->gateway_ = ntohl(addr.s_addr);
		entry->intfName_ = intf;
		return true;
	}
	return false;
}

bool GDemonServer::RtmFunc::checkD(char* buf, RtmEntry* entry) {
	char cidr[256];
	char intf[256];
	char ip[256];
	// 10.2.2.0/24 dev wlan0  proto kernel  scope link  src 10.2.2.189 (D)
	int res = sscanf(buf, "%s dev %s proto kernel scope link src %s", cidr, intf, ip);
	if (res == 3) {
		GTRACE("checkD %s", buf); // gilgil temp
		uint32_t dst;
		uint32_t mask;
		if (!decodeCidr(cidr, &dst, &mask)) return false;
		entry->dst_ = dst;
		entry->mask_ = mask;
		entry->intfName_ = intf;
		return true;
	}
	return false;
}

bool GDemonServer::RtmFunc::decodeCidr(std::string cidr, uint32_t* dst, uint32_t* mask) {
	size_t found = cidr.find("/");
	if (found == std::string::npos) return false;
	std::string dstStr = cidr.substr(0, found);
	struct in_addr addr;
	inet_aton(dstStr.data(), &addr);
	*dst = ntohl(addr.s_addr);
	std::string maskStr = cidr.substr(found + 1);
	*mask = numberToMask(std::stoi(maskStr.data()));
	return true;
}

uint32_t GDemonServer::RtmFunc::numberToMask(int number) {
	uint32_t res = 0;
	for (int i = 0; i < number; i++) {
		res = (res >> 1);
		res |= 0x80000000;
	}
	return res;
}
