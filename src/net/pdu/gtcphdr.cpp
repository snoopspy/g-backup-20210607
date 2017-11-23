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
GPdu* GTcpParser::doParse(GPacket* packet, GPdu* prev) {
  switch (prev->pduType()) {
    case GPdu::Type::Ip: {
        Q_ASSERT(dynamic_cast<GIpHdr*>(prev) != nullptr);
        GIpHdr* ipHdr = (GIpHdr*)prev;
        if (ipHdr->p() != IPPROTO_TCP)
          return nullptr;
        if (packet->parse_.size_ < sizeof(TCP_HDR))
          return nullptr;
        return new GTcpHdr(packet->parse_.data_);
      }
    case GPdu::Type::Ip6:
      // TODO process ipv6 header // gilgil temp 2016.09.13
      return nullptr;
    default:
      return nullptr;
  }
}
