#include "gpcapdevicewriter.h"

// ----------------------------------------------------------------------------
// GPcapDeviceWriter
// ----------------------------------------------------------------------------
GPcapDeviceWriter::GPcapDeviceWriter(QObject* parent) : GPcapDevice(parent) {
  dev_ = "";
  autoRead_= false;
  autoParse_ = false;
}

GPcapDeviceWriter::~GPcapDeviceWriter() {
}

GCapture::Result GPcapDeviceWriter::write(GPacket* packet) {
  return GPcapDevice::write(packet);
}
