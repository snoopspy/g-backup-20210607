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
		Session* session = new Session;
		session->sd_ = new_sd;
		session->t_ = new std::thread(Session::_run, &sessions_, session);
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

void GDemonServer::Session::_run(SessionList* sessions, Session* session) {
	sessions->lock();
	sessions->push_back(session);
	sessions->unlock();

	session->run();

	sessions->lock();
	sessions->remove(session);
	sessions->unlock();
}

void GDemonServer::Session::run() {
	GTRACE("run beg");

	bool active = true;
	while (active) {
		int32_t readLen;
		if (!readAll(sd_, &readLen, sizeof(readLen)))
			break;

		char buffer[MaxBufferSize];
		assert(readLen < MaxBufferSize);
		if (!readAll(sd_, buffer, readLen))
			break;

		pchar buf = buffer;
		int32_t cmd = *pint32_t(buf); buf += sizeof(cmd); readLen -= sizeof(cmd);
		switch (cmd) {
			case cmdGetInterfaceList:
				active = processGetInterfaceList(buf, readLen);
				break;
			case cmdPcapOpen:
				active = processPcapOpen(buf, readLen);
				break;
			case cmdPcapClose:
				active = processPcapClose(buf, readLen);
				break;
			default:
				GTRACE("invalid cmd %d", cmd);
				active = false;
				break;
		}
	}

	::close(sd_);
	if (pcap_ != nullptr)
		pcap_close(pcap_);

	GTRACE("run end");
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
	GTRACE("processGetInterfaceList"); // gilgil temp 2021.02.26

	pcap_if_t* allDevs;
	char errBuf[PCAP_ERRBUF_SIZE];
	int i = pcap_findalldevs(&allDevs, errBuf);
	if (i != 0) { // if error occured
		GTRACE("error in pcap_findalldevs_ex (%s)", errBuf);
		return false;
	}

	//
	// Add all interfaces
	//
	pcap_if_t* dev = allDevs;
	InterfaceList interfaceList;
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
		interfaceList.push_back(interface);
		dev = dev->next;
		i++;
	}
	pcap_freealldevs(allDevs);

	char buffer[MaxBufferSize];
	pchar buf = buffer + sizeof(int32_t);
	int32_t writeLen = interfaceList.encode(buf, MaxBufferSize - sizeof(int32_t));
	*pint32_t(buffer) = writeLen; writeLen += sizeof(writeLen);

	int res = ::send(sd_, buffer, writeLen, 0);
	if (res == 0 || res == -1) {
		GTRACE("send return %d", res);
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
