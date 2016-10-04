#include "gudphdr.h"
#include "giphdr.h"
#include "net/packet/gpacket.h"

// ----------------------------------------------------------------------------
// GUdpHdr
// ----------------------------------------------------------------------------
size_t GUdpHdr::size() {
  return sizeof(UDP_HDR);
}

GUdpHdr::GUdpHdr(u_char* buf) {
  udp_hdr_ = (UDP_HDR*)buf;
}

// ----------------------------------------------------------------------------
// GUdpParser
// ----------------------------------------------------------------------------
bool GUdpParser::isMatch(GPdu* prev, GPacket* packet) {
  switch (prev->pduType()) {
    case GPduType::Ip: {
        Q_ASSERT(dynamic_cast<GIpHdr*>(prev) != nullptr);
        GIpHdr* ipHdr = (GIpHdr*)prev;
        if (ipHdr->p() != IPPROTO_UDP)
          return false;
        if (packet->parse_.size_ < sizeof(UDP_HDR))
          return false;
        return true;
      }
    case GPduType::Ip6:
      // TODO process ipv6 header // gilgil temp 2016.09.13
      return false;
    default:
      return false;
  }
}

GPdu* GUdpParser::doParse(GPacket* packet) {
  Q_ASSERT(packet->parse_.size_ >= sizeof(UDP_HDR));
  return new GUdpHdr(packet->parse_.data_);
}
