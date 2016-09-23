#include "gpcapdevicewriter.h"

// ----------------------------------------------------------------------------
// GPcapDeviceWriter
// ----------------------------------------------------------------------------
GPcapDeviceWriter::GPcapDeviceWriter(QObject* parent) : GPcapDevice(parent) {
  this->autoRead_= false;
}

GPcapDeviceWriter::~GPcapDeviceWriter() {
}

GCapture::Result GPcapDeviceWriter::write(GPacket* packet) {
  return GPcapDevice::write(packet);
}
