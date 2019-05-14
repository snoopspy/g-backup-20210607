#include "gethparser.h"
#include "net/packet/gethpacket.h"
#include "net/packet/gippacket.h"

// ----------------------------------------------------------------------------
// GEthParser
// ----------------------------------------------------------------------------
void GEthParser::parse(GPacket* packet) {
  GEthPacket* ethPacket = static_cast<GEthPacket*>(packet);
  ethPacket->ethHdr_ = reinterpret_cast<GEthHdr*>(packet->buf_.data_);
  if (ethPacket->ethHdr_->type() == GEthHdr::Ip) {
    GBuf backup = ethPacket->buf_;
    ethPacket->buf_.data_ += sizeof(GEthHdr);
    ethPacket->buf_.size_ -= sizeof(GEthHdr);
    GIpParser::parse(packet);
    ethPacket->buf_ = backup;
  }
}
