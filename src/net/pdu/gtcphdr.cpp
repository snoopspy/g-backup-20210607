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
GPdu* GTcpParser::doParse(GPacket* packet) {
  if (packet->parse_.size_ < sizeof(TCP_HDR))
    return nullptr;
  return new GTcpHdr(packet->parse_.data_);
}
