#include "gpacket.h"
#include "net/pdu/gpdu.h"

// ----------------------------------------------------------------------------
// GPacket
// ----------------------------------------------------------------------------
GPacket::GPacket() {
  capture_ = nullptr;
  clear();
}

GPacket::GPacket(GCapture* capture) {
  capture_ = capture;
  clear();
}

GPacket::~GPacket() {
}

void GPacket::clear() {
  memset(&pkthdr_, 0, sizeof(pkthdr_));
  buf_ = nullptr;
  control.block_ = false;
  parseBuf_ = nullptr;
  parseLen_ = 0;
  pdus_.clear();
}
