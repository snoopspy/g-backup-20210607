#include "gpacket.h"
#include "net/pdu/gpdu.h"

// ----------------------------------------------------------------------------
// GPacket
// ----------------------------------------------------------------------------
GPacket::GPacket() {
  capture_ = nullptr;
  pdus_ = new GPdus;
  clear();
}

GPacket::GPacket(GCapture* capture) {
  capture_ = capture;
  pdus_ = new GPdus;
  clear();
}

GPacket::~GPacket() {
  if (pdus_ != nullptr) {
    delete pdus_;
    pdus_ = nullptr;
  }
}

void GPacket::clear() {
  memset(&ts_, 0, sizeof(ts_));
  buf_ = nullptr;
  len_ = 0;
}

// ----------------------------------------------------------------------------
// GTEST
// ----------------------------------------------------------------------------
#ifdef GTEST
#include <gtest/gtest.h>

TEST(GPacket, basicTest) {

}

#endif // GTEST
