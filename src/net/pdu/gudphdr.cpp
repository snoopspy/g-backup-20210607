#include "gudphdr.h"
#include "giphdr.h"
#include "net/packet/gpacket.h"

// ----------------------------------------------------------------------------
// GUdpHdr
// ----------------------------------------------------------------------------
size_t GUdpHdr::size() {
  Q_ASSERT(udp_hdr_ != nullptr);
  return sizeof(UDP_HDR);
}

GUdpHdr::GUdpHdr(u_char* buf) {
  udp_hdr_ = (UDP_HDR*)buf;
}

// ----------------------------------------------------------------------------
// GUdpParser
// ----------------------------------------------------------------------------
GPdu* GUdpParser::doParse(GPacket* packet) {
  if (packet->parse_.size_ < sizeof(UDP_HDR))
    return nullptr;
  return new GUdpHdr(packet->parse_.data_);
}
