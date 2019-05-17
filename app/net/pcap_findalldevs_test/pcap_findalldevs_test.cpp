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
  char s[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &addr_in->sin_addr, s, INET_ADDRSTRLEN);
  clog << s;
}

void debug_ip6(struct sockaddr* addr) {
  sockaddr_in6* addr6_in = reinterpret_cast<sockaddr_in6*>(addr);
  char s[INET6_ADDRSTRLEN];
  inet_ntop(AF_INET6, &addr6_in->sin6_addr, s, INET6_ADDRSTRLEN);
  clog << s;
}

void debug(struct sockaddr* addr) {
  clog << "sa_family " << addr->sa_family << " ";
  switch (addr->sa_family) {
    case AF_INET:   clog << "AF_INET  "; debug_ip4(addr); break;
    case AF_INET6:  clog << "AF_INET6 "; debug_ip6(addr); break;
    // case AF_PACKET: clog << "AF_PACKET "; break;
    default : clog << "? "; break;
  }
  clog << endl;
  //sockaddr_in* addr_in = reinterpret_cast<sockaddr_in*>(addr);
//      ntohl(addr_in->sin_addr.s_addr);

}

void debug(pcap_addr_t* pa) {
  while (pa != nullptr) {
    if (pa->addr != nullptr)      { clog << "  addr      "; debug(pa->addr); }
    if (pa->netmask != nullptr)   { clog << "  netmask   "; debug(pa->netmask); }
    if (pa->broadaddr != nullptr) { clog << "  broadaddr "; debug(pa->broadaddr); }
    if (pa->dstaddr != nullptr)   { clog << "  dstaddr   "; debug(pa->dstaddr); }
    clog << "  " << endl;
    pa = pa->next;
  }
}

void debug(pcap_if_t* dev) {
  if (dev->name != nullptr)        clog << "name        : " << dev->name << endl;
  if (dev->description != nullptr) clog << "description : " << dev->description << endl;
  if (dev->addresses != nullptr) debug(dev->addresses);
  clog << "flags " << dev->flags << endl;
}

int main() {
  pcap_if_t* allDevs_;
  char errBuf[PCAP_ERRBUF_SIZE];
  int i = pcap_findalldevs(&allDevs_, errBuf);
  if (i != 0) {
    clog << "error in pcap_findalldevs " << errBuf << endl;
    return -1;
  }

  i = 0;
  pcap_if_t* dev = allDevs_;
  while (dev != nullptr) {
    clog << i << " ================================" << endl;
    debug(dev);
    dev = dev->next;
  }
}
