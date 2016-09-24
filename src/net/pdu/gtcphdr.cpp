#include "gtcphdr.h"
#include "giphdr.h"
#include "net/packet/gpacket.h"

// ----------------------------------------------------------------------------
// GTcpHdr
// ----------------------------------------------------------------------------
size_t GTcpHdr::size() {
  Q_ASSERT(tcp_hdr_ != nullptr);
  return tcp_hdr_->th_off * 4;
}

GTcpHdr::GTcpHdr(u_char* buf) {
  tcp_hdr_ = (TCP_HDR*)buf;
}

// ----------------------------------------------------------------------------
// GTcpParser
// ----------------------------------------------------------------------------
bool GTcpParser::isMatch(GPdu* prev, GPacket* packet) {
  switch (prev->pduType()) {
    case GPduType::Ip: {
        Q_ASSERT(dynamic_cast<GIpHdr*>(prev) != nullptr);
        GIpHdr* ipHdr = (GIpHdr*)prev;
        if (ipHdr->p() != IPPROTO_TCP)
          return false;
        if (packet->parseLen_ < sizeof(TCP_HDR))
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

GPdu* GTcpParser::doParse(GPacket* packet) {
  Q_ASSERT(packet->parseLen_ >= sizeof(TCP_HDR));
  return new GTcpHdr(packet->parseBuf_);
}
