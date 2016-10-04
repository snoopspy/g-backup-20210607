#include "gethhdr.h"
#include "net/packet/gpacket.h"

// ----------------------------------------------------------------------------
// GEthHdr
// ----------------------------------------------------------------------------
size_t GEthHdr::size() {
  Q_ASSERT(eth_hdr_ != nullptr);
  return sizeof(ETH_HDR);
}

GEthHdr::GEthHdr(u_char* buf) {
  eth_hdr_ = (ETH_HDR*)buf;
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
  if ((packet->parse_.size_) < sizeof(ETH_HDR))
    return nullptr;
  return new GEthHdr(packet->parse_.data_);
}
