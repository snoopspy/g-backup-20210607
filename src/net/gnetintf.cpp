#include <QDebug>
#include "gnetintf.h"
#include "grtm.h"

// ----------------------------------------------------------------------------
// GNetInft
// ----------------------------------------------------------------------------
GNetIntfs& GNetIntf::all() {
  return GNetIntfs::instance();
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
    qWarning() << QString("error in pcap_findalldevs_ex (%s)").arg(errBuf);
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
    intf.description_ = dev->description;
    intf.dev_ = dev;

    for(pcap_addr_t* pa = dev->addresses; pa != nullptr; pa = pa->next) {
      // ip_
      sockaddr* addr = pa->addr;
      sockaddr_in* addr_in = reinterpret_cast<sockaddr_in*>(addr);
      if(addr != nullptr && addr->sa_family == AF_INET)
        intf.ip_ = ntohl(addr_in->sin_addr.s_addr);

      // mask_;
      addr = pa->netmask;
      addr_in = reinterpret_cast<sockaddr_in*>(addr);
      if(addr != nullptr && addr->sa_family == AF_INET) {
        intf.mask_ = ntohl(addr_in->sin_addr.s_addr);
      }
    }

#ifdef Q_OS_LINUX
    // mac
    intf.mac_ = getMac(dev->name);

    // gateway_
    intf.gateway_ = GRtm::instance().findGateway(intf.name_, intf.ip_);
#endif
#ifdef Q_OS_WIN
    PIP_ADAPTER_INFO adapter = GAdapterInfos::all().findByName(intf.name_);
    if (adapter != nullptr) {
      intf.mac_ = adapter->Address;
      intf.gateway_ = static_cast<char*>(adapter->GatewayList.IpAddress.String);
    }
#endif

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
  if (allDevs_ != nullptr)
  {
    pcap_freealldevs(allDevs_);
    allDevs_ = nullptr;
  }
}

GNetIntf* GNetIntfs::findByName(QString name) {
  for (GNetIntf& intf: *this) {
    if (intf.name_ == name)
      return &intf;
  }
  return nullptr;
}

GNetIntfs& GNetIntfs::instance() {
  static GNetIntfs intfs;
  return intfs;
}

// ----------------------------------------------------------------------------
// GAdapterInfo
// ----------------------------------------------------------------------------
#ifdef Q_OS_WIN
GAdapterInfos::GAdapterInfos() {
  ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
  pAdapterInfo = PIP_ADAPTER_INFO(malloc(sizeof(IP_ADAPTER_INFO)));
  if (pAdapterInfo == nullptr) {
    qCritical() << "Error allocating memory needed to call GetAdaptersinfo";
    return;
  }

  if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
    free(pAdapterInfo);
    pAdapterInfo = PIP_ADAPTER_INFO(malloc(ulOutBufLen));
    if (pAdapterInfo == nullptr) {
      qCritical() << "Error allocating memory needed to call GetAdaptersinfo";
      return;
    }
  }

  DWORD dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
  if (dwRetVal != NO_ERROR) {
    qCritical() << "GetAdaptersInfo failed with error: " << dwRetVal;
  }
}

GAdapterInfos::~GAdapterInfos() {
  if (pAdapterInfo != nullptr) {
    free(pAdapterInfo);
    pAdapterInfo = nullptr;
  }
}

PIP_ADAPTER_INFO GAdapterInfos::findByName(QString name) {
  PIP_ADAPTER_INFO res = pAdapterInfo;
  while (res != nullptr) {
    QString adapterName(res->AdapterName);
    if (adapterName.indexOf(name) != -1)
      return res;
  }
  return nullptr;
}

GAdapterInfos& GAdapterInfos::all() {
  static GAdapterInfos adapterInfos;
  return adapterInfos;
}

#endif
