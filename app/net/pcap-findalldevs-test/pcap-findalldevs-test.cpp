#ifdef __linux
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <netinet/in.h>
	#include <sys/socket.h>
	#include <unistd.h>
#endif
#ifdef WIN32
	#ifndef NOMINMAX
	#define NOMINMAX
	#endif // NOMINMAX
	#include <winsock2.h>
	#include <windows.h>
	#include <ws2tcpip.h>
#endif
#include <pcap.h>
#include <iostream>
#include <string>

using namespace std;

void debug_ip4(struct sockaddr* addr) {
	sockaddr_in* addr_in = reinterpret_cast<sockaddr_in*>(addr);
#ifdef __linux__
	char s[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &addr_in->sin_addr, s, INET_ADDRSTRLEN);
#endif // __linux__
#ifdef WIN32
	char* s = inet_ntoa(addr_in->sin_addr);
#endif // WIN32
	cout << s;
}

void debug_ip6(struct sockaddr* addr) {
	sockaddr_in6* addr6_in = reinterpret_cast<sockaddr_in6*>(addr);
#ifdef __linux__
	char s[INET6_ADDRSTRLEN];
	inet_ntop(AF_INET6, &addr6_in->sin6_addr, s, INET6_ADDRSTRLEN);
#endif // __linux__
#ifdef WIN32
	(void)addr6_in;
	const char* s = "not implemented yet"; // gilgil temp 2020.10.27
#endif // WIN32
	cout << s;
}

void debug(struct sockaddr* addr) {
	cout << "sa_family " << addr->sa_family << " ";
	switch (addr->sa_family) {
		case AF_INET: cout << "AF_INET "; debug_ip4(addr); break;
		case AF_INET6: cout << "AF_INET6 "; debug_ip6(addr); break;
	// case AF_PACKET: cout << "AF_PACKET "; break;
		default : cout << "? "; break;
	}
	cout << endl;
}

void debug(pcap_addr_t* pa) {
	while (pa != nullptr) {
		if (pa->addr != nullptr)      { cout << "  addr      "; debug(pa->addr); }
		if (pa->netmask != nullptr)   { cout << "  netmask   "; debug(pa->netmask); }
		if (pa->broadaddr != nullptr) { cout << "  broadaddr "; debug(pa->broadaddr); }
		if (pa->dstaddr != nullptr)   { cout << "  dstaddr   "; debug(pa->dstaddr); }
		cout << "  " << endl;
		pa = pa->next;
	}
}

void debug(pcap_if_t* dev) {
	if (dev->name != nullptr)        cout << "name        : " << dev->name << endl;
	if (dev->description != nullptr) cout << "description : " << dev->description << endl;
	if (dev->addresses != nullptr) debug(dev->addresses);
	cout << "flags " << dev->flags << endl;
}

int main() {
	pcap_if_t* allDevs_;
	char errBuf[PCAP_ERRBUF_SIZE];
	int i = pcap_findalldevs(&allDevs_, errBuf);
	if (i != 0) {
		cout << "error in pcap_findalldevs " << errBuf << endl;
		return -1;
	}

	i = 0;
	pcap_if_t* dev = allDevs_;
	while (dev != nullptr) {
		cout << i++ << " ================================" << endl;
		debug(dev);
		dev = dev->next;
	}
}
