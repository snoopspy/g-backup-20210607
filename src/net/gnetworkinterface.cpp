#include <QDebug>
#include "gnetworkinterface.h"

// ----------------------------------------------------------------------------
// GNetworkInterface
// ----------------------------------------------------------------------------
GNetworkInterfaces& GNetworkInterface::allInterfaces() {
  return GNetworkInterfaces::instance();
}

// ----------------------------------------------------------------------------
// GNetworkInterfaces
// ----------------------------------------------------------------------------
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
static GMac getMac(char* name) {
  int s;
  struct ifreq buffer;

  s = socket(PF_INET, SOCK_DGRAM, 0);
  memset(&buffer, 0x00, sizeof(buffer));

  strcpy(buffer.ifr_name, name);
  ioctl(s, SIOCGIFHWADDR, &buffer);
  close(s);

  GMac res = (uint8_t*)buffer.ifr_ifru.ifru_hwaddr.sa_data;
  return res;
}

GNetworkInterfaces::GNetworkInterfaces() {
  //
  // Initialize allDevs using pcap API.
  //
  if (allDevs_ != nullptr) return;

  char errBuf[PCAP_ERRBUF_SIZE];

  int i = pcap_findalldevs(&allDevs_, errBuf);
  if (i != 0) { // if error occured
    qWarning() << QString("error in pcap_findalldevs_ex (%s)").arg(errBuf);
    return;
  }

  //
  // Add all interfaces
  //
  pcap_if_t* dev = allDevs_;
  i = 1;
  while (dev != NULL)
  {
    GNetworkInterface intf;

    intf.index_ = i;
    intf.name_ = dev->name;
    intf.description_ = dev->description;
    intf.dev_ = dev;

    for(pcap_addr_t* pa = dev->addresses; pa != nullptr; pa = pa->next) {
      sockaddr* addr;

      // ip_
      addr = pa->addr;
      if(addr != nullptr && addr->sa_family == AF_INET)
        intf.ip_ = ((struct sockaddr_in*)addr)->sin_addr;

      // subnet_
      addr = pa->netmask;
      if(addr != nullptr && addr->sa_family == AF_INET) {
        intf.subnet_ = ((struct sockaddr_in*)addr)->sin_addr;

      intf.mac_ = getMac(dev->name);
      }
    }

    push_back(intf);
    dev = dev->next;
    i++;
  }
}

GNetworkInterfaces::~GNetworkInterfaces() {
  clear();

  //
  // Finalize allDevs_
  //
  if (allDevs_ != NULL)
  {
    pcap_freealldevs(allDevs_);
    allDevs_ = nullptr;
  }
}

GNetworkInterfaces& GNetworkInterfaces::instance() {
  static GNetworkInterfaces intfs;
  return intfs;
}
