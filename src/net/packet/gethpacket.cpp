#include "gethpacket.h"

// ----------------------------------------------------------------------------
// GEthPacket
// ----------------------------------------------------------------------------
void GEthPacket::parse() {
  ethHdr_ = reinterpret_cast<GEthHdr*>(buf_.data_);
  switch (ethHdr_->type()) {
    case GEthHdr::Ip4:
    case GEthHdr::Ip6: {
      GBuf backup = buf_;
      buf_.data_ += sizeof(GEthHdr);
      buf_.size_ -= sizeof(GEthHdr);
      GIpPacket::parse();
      buf_ = backup;
      break;
    }
    case GEthHdr::Arp:
      arpHdr_ = reinterpret_cast<GArpHdr*>(buf_.data_ + sizeof(GEthHdr));
      break;
  }
}
