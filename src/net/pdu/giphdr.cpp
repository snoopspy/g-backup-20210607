#include "giphdr.h"
#include "gethhdr.h"
#include "net/packet/gpacket.h"

// ----------------------------------------------------------------------------
// GIpHdr
// ----------------------------------------------------------------------------
size_t GIpHdr::size() {
  Q_ASSERT(ip_hdr_ != nullptr);
  return ip_hdr_->ip_hl * 4;
}

GIpHdr::GIpHdr(u_char* buf) {
  ip_hdr_ = (IP_HDR*)buf;
}

// ----------------------------------------------------------------------------
// GIpParser
// ----------------------------------------------------------------------------
GPdu* GIpParser::doParse(GPacket* packet) {
  if (packet->parse_.size_ < sizeof(IP_HDR))
    return nullptr;
#ifdef _DEBUG
  if (*packet->parse_.data_ != 0x45) {
    qWarning() << QString("packet start is not 0x45 0x%1").arg(QString::number(*packet->parse_.data_, 16));
  }
#endif // _DEBUG
  return new GIpHdr(packet->parse_.data_);
}
