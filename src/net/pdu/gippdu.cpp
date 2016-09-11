#include "gippdu.h"
#include "net/packet/gpacket.h"
#include "gethpdu.h"

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
  ETH_HDR* ethHdr = ((GEthPdu*)prev)->ethHdr_;
  if (ntohs(ethHdr->ether_type) != ETHERTYPE_IP)
    return false;

  if (packet->len_ <= sizeof(IP_HDR))
    return false;

  return true;
}

GPdu* GIpParser::doParse(GPacket* packet) {
  Q_ASSERT(packet->len_ >= sizeof(IP_HDR));
  return new GIpPdu(packet->buf_);
}

