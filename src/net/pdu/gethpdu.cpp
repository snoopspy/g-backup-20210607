#include "gethpdu.h"

// ----------------------------------------------------------------------------
// GEthPdu
// ----------------------------------------------------------------------------
size_t GEthPdu::size() {
  Q_ASSERT(ethHdr_ != nullptr);
  return sizeof(ETH_HDR);
}

GEthPdu::GEthPdu(u_char* buf) {
  ethHdr_ = (ETH_HDR*)buf;
}

// ----------------------------------------------------------------------------
// GEthParser
// ----------------------------------------------------------------------------
bool GEthParser::isMatch(GPdu* prev, GPacket* packet) {
  (void)prev;
  (void)packet;
  SET_ERR(GErr::UNREACHABLE_CODE, "unreachable code");
  return false;
}

GPdu* GEthParser::doParse(GPacket* packet) {
  if ((packet->len_) < sizeof(ETH_HDR))
    return nullptr;
  return new GEthPdu(packet->buf_);
}
