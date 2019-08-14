#include "garpspoof.h"

// ----------------------------------------------------------------------------
// GArpSpoof
// ----------------------------------------------------------------------------
GArpSpoof::GArpSpoof(QObject* parent) : GPcapDevice(parent) {

}

GArpSpoof::~GArpSpoof() {
  close();
}

bool GArpSpoof::doOpen() {
  return GPcapDevice::doOpen();
}

bool GArpSpoof::doClose() {
  return GPcapDevice::doClose();
}
