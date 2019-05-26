#include "gethpacket.h"

// ----------------------------------------------------------------------------
// GEthPacket
// ----------------------------------------------------------------------------
void GEthPacket::parse() {
  ethHdr_ = reinterpret_cast<GEthHdr*>(buf_.data_);
  switch (ethHdr_->type()) {
    case GEthHdr::IP4:
    case GEthHdr::IP6: {
      GBuf backup = buf_;
      buf_.data_ += sizeof(GEthHdr);
      buf_.size_ -= sizeof(GEthHdr);
      GIpPacket::parse();
      buf_ = backup;
      break;
    }
    case GEthHdr::ARP:
      arpHdr_ = reinterpret_cast<GArpHdr*>(buf_.data_ + sizeof(GEthHdr));
      break;
  }
}
