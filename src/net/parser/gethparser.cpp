#include "gethparser.h"
#include "net/packet/gethpacket.h"

// ----------------------------------------------------------------------------
// GEthParser
// ----------------------------------------------------------------------------
void GEthParser::parse(GPacket* packet) {
  Q_ASSERT(dynamic_cast<GEthPacket*>(packet) != nullptr);
  GEthPacket* ethPacket = static_cast<GEthPacket*>(packet);

  ethPacket->ethHdr_ = reinterpret_cast<GEthHdr*>(packet->buf_.data_);
  switch (ethPacket->ethHdr_->type()) {
    case GEthHdr::Ip:
    case GEthHdr::Ip6: {
      GBuf backup = ethPacket->buf_;
      ethPacket->buf_.data_ += sizeof(GEthHdr);
      ethPacket->buf_.size_ -= sizeof(GEthHdr);
      GIpParser::parse(packet);
      ethPacket->buf_ = backup;
      break;
    }
    case GEthHdr::Arp:
      ethPacket->arpHdr_ = reinterpret_cast<GArpHdr*>(packet->buf_.data_);
      break;
  }
}
