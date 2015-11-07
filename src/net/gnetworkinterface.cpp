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
