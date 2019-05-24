#include "gpcapdevicewriter.h"
#include "base/prop/gpropitem_device.h"

// ----------------------------------------------------------------------------
// GPcapDeviceWriter
// ----------------------------------------------------------------------------
bool GPcapDeviceWriter::doOpen() {
  if (devName_ == "") {
    SET_ERR(GErr::DEVICE_NOT_SPECIFIED, "device is not specified");
    return false;
  }

  char errBuf[PCAP_ERRBUF_SIZE];
  pcap_ = pcap_open_live(qPrintable(devName_), 0, 0, 0, errBuf);
  if (pcap_ == nullptr) {
    SET_ERR(GErr::RETURN_NULL, errBuf);
    return false;
  }

  return true;
}

bool GPcapDeviceWriter::doClose()  {
  if (pcap_ != nullptr) {
    pcap_close(pcap_);
    pcap_ = nullptr;
  }
  return true;
}

GPacket::Result GPcapDeviceWriter::write(GPacket* packet) {
  int i = pcap_sendpacket(pcap_, packet->buf_.data_, int(packet->buf_.size_));
  if (i == 0) {
    emit written(packet);
    return GPacket::Ok;
  }
  qWarning() << QString("pcap_sendpacket return %1").arg(i);
  return GPacket::Fail;
}

#ifdef QT_GUI_LIB

GPropItem* GPcapDeviceWriter::propCreateItem(GPropItemParam* param) {
  if (QString(param->mpro_.name()) == "devName") {
    GPropItemDevice* res = new GPropItemDevice(param);
    return res;
  }
  return GObj::propCreateItem(param);
}

#endif // QT_GUI_LIB
