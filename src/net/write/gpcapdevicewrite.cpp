#include "gpcapdevicewrite.h"
#include "base/prop/gpropitem_device.h"

// ----------------------------------------------------------------------------
// GPcapDeviceWrite
// ----------------------------------------------------------------------------
bool GPcapDeviceWrite::doOpen() {
  if (devName_ == "") {
    SET_ERR(GErr::DEVICE_NOT_SPECIFIED, "device is not specified");
    return false;
  }
  intf_ = GNetIntf::all().findByName(devName_);

  char errBuf[PCAP_ERRBUF_SIZE];
  pcap_ = pcap_open_live(qPrintable(devName_), 0, 0, 0, errBuf);
  if (pcap_ == nullptr) {
    SET_ERR(GErr::RETURN_NULL, errBuf);
    return false;
  }

  int dataLink = pcap_datalink(pcap_);
  dataLinkType_ = GPacket::intToDataLinkType(dataLink);

  return true;
}

bool GPcapDeviceWrite::doClose()  {
  if (pcap_ != nullptr) {
    pcap_close(pcap_);
    pcap_ = nullptr;
  }

  intf_ = nullptr;

  return true;
}

GPacket::Result GPcapDeviceWrite::write(GPacket* packet) {
  GBuf buf = GPacket::getWriteBuf(packet->buf_, packet->dataLinkType_, dataLinkType_);
  if (!buf.valid()) {
    SET_ERR(GErr::NOT_SUPPORTED, "invalid data link type");
    return GPacket::Fail;
  }
  int i = pcap_sendpacket(pcap_, buf.data_, int(buf.size_));
  if (i == 0) {
    emit written(packet);
    return GPacket::Ok;
  }
  SET_ERR(GErr::FAIL, QString("pcap_sendpacket return %1").arg(i));
  return GPacket::Fail;
}

#ifdef QT_GUI_LIB

GPropItem* GPcapDeviceWrite::propCreateItem(GPropItemParam* param) {
  if (QString(param->mpro_.name()) == "devName") {
    GPropItemDevice* res = new GPropItemDevice(param);
    return res;
  }
  return GObj::propCreateItem(param);
}

#endif // QT_GUI_LIB
