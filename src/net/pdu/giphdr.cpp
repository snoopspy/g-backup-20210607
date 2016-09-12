#include "giphdr.h"
#include "gethhdr.h"
#include "net/packet/gpacket.h"

// ----------------------------------------------------------------------------
// GIpHdr
// ----------------------------------------------------------------------------
size_t GIpHdr::size() {
  Q_ASSERT(ip_hdr_ != nullptr);
  return (ip_hdr_->ip_hl) * 4;
}

GIpHdr::GIpHdr(u_char* buf) {
  ip_hdr_ = (IP_HDR*)buf;
}

// ----------------------------------------------------------------------------
// GIpParser
// ----------------------------------------------------------------------------
bool GIpParser::isMatch(GPdu* prev, GPacket* packet) {
  Q_ASSERT(dynamic_cast<GEthHdr*>(prev) != nullptr);
  GEthHdr* ethHdr = (GEthHdr*)prev;

  if (ethHdr->type() != ETHERTYPE_IP)
    return false;

  if (packet->len_ < sizeof(IP_HDR))
    return false;

  return true;
}

GPdu* GIpParser::doParse(GPacket* packet) {
  Q_ASSERT(packet->len_ >= sizeof(IP_HDR));
  return new GIpHdr(packet->buf_);
}
