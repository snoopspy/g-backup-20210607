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
  memset(&ts_, 0, sizeof(ts_));
  buf_ = nullptr;
  len_ = 0;
  pdus_.clear();
}
