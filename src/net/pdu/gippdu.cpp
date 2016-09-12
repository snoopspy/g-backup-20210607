#include "gippdu.h"
#include "gethpdu.h"
#include "net/packet/gpacket.h"

// ----------------------------------------------------------------------------
// GIpPdu
// ----------------------------------------------------------------------------
size_t GIpPdu::size() {
  Q_ASSERT(ipHdr_ != nullptr);
  return (ipHdr_->ip_v) * 4;
}

GIpPdu::GIpPdu(u_char* buf) {
  ipHdr_ = (IP_HDR*)buf;
}

// ----------------------------------------------------------------------------
// GIpParser
// ----------------------------------------------------------------------------
bool GIpParser::isMatch(GPdu* prev, GPacket* packet) {
  Q_ASSERT(dynamic_cast<GEthPdu*>(prev) != nullptr);
  GEthPdu* ethPdu = (GEthPdu*)prev;

  if (ethPdu->type() != ETHERTYPE_IP)
    return false;

  if (packet->len_ <= sizeof(IP_HDR))
    return false;

  return true;
}

GPdu* GIpParser::doParse(GPacket* packet) {
  Q_ASSERT(packet->len_ >= sizeof(IP_HDR));
  return new GIpPdu(packet->buf_);
}

