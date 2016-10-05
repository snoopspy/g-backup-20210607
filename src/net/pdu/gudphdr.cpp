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
GPdu* GUdpParser::doParse(GPacket* packet, GPdu* prev) {
  switch (prev->pduType()) {
    case GPduType::Ip: {
        Q_ASSERT(dynamic_cast<GIpHdr*>(prev) != nullptr);
        GIpHdr* ipHdr = (GIpHdr*)prev;
        if (ipHdr->p() != IPPROTO_UDP)
          return nullptr;
        if (packet->parse_.size_ < sizeof(UDP_HDR))
          return nullptr;
        return new GUdpHdr(packet->parse_.data_);
      }
    case GPduType::Ip6:
      // TODO process ipv6 header // gilgil temp 2016.09.13
      return nullptr;
    default:
      return nullptr;
  }
}
