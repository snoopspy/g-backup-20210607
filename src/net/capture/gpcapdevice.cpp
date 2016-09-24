#include "gpcapdevice.h"
#include "net/grtm.h"
#include "base/prop/gpropitem_device.h"

// ----------------------------------------------------------------------------
// GPcapDevice
// ----------------------------------------------------------------------------
GPcapDevice::GPcapDevice(QObject* parent) : GPcap(parent) {
  GRtmEntry* entry = GRtm::instance().getBestEntry("8.8.8.8");
  if (entry != nullptr)
    dev_ = entry->intf_;
}

GPcapDevice::~GPcapDevice() {
  close();
}

bool GPcapDevice::doOpen() {
  if (dev_ == "") {
    SET_ERR(GErr::DEVICE_NOT_SPECIFIED, "device is not specified");
    return false;
  }

  char errBuf[PCAP_ERRBUF_SIZE];
  pcap_ = pcap_open_live(qPrintable(dev_), snapLen_, flags_, readTimeout_, errBuf);
  if (pcap_ == nullptr) {
    SET_ERR(GErr::RETURN_NULL, errBuf);
    return false;
  }

  return GPcap::doOpen();
}

bool GPcapDevice::doClose() {
  return GPcap::doClose();
}

#ifdef QT_GUI_LIB

GPropItem* GPcapDevice::propCreateItem(GPropItemParam param) {
  if (QString(param.mpro_.name()) == "dev") {
    GPropItemDevice* res = new GPropItemDevice(param);
    return res;
  }
  return GObj::propCreateItem(param);
}

#endif // QT_GUI_LIB
