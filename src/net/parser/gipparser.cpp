#include "gipparser.h"
#include "net/packet/gippacket.h"

// ----------------------------------------------------------------------------
// GIpParser
// ----------------------------------------------------------------------------
void GIpParser::parse(GPacket* packet) {
  GIpPacket* ipPacket = static_cast<GIpPacket*>(packet);

  u_char* p = ipPacket->buf_.data_;
  switch (*p & 0xF0) {
    case 0x40: // version 4
      ipPacket->ipHdr_ = reinterpret_cast<GIpHdr*>(p);
      p += ipPacket->ipHdr_->hl() * 4;
      break;
    case 0x60: // /version 6
      ipPacket->ip6Hdr_= reinterpret_cast<GIp6Hdr*>(p);
      return; // gilgil temp 2019.05.14
    default:
      qWarning() << "invalid ip header version" << uint8_t(*p);
      return;
  }

  switch (ipPacket->ipHdr_->p()) {
    case GIpHdr::Tcp: // Tcp
      ipPacket->tcpHdr_ = reinterpret_cast<GTcpHdr*>(p);
      p += ipPacket->tcpHdr_->off() * 4;
      ipPacket->tcpData_ = GTcpHdr::parseData(ipPacket->ipHdr_, ipPacket->tcpHdr_);
      break;
    case GIpHdr::Udp: // Udp
      ipPacket->udpHdr_ = reinterpret_cast<GUdpHdr*>(p);
      p += sizeof(GUdpHdr);
      ipPacket->udpData_ = GUdpHdr::parseData(ipPacket->udpHdr_);
      break;
    case GIpHdr::Icmp: // Idmp
      ipPacket->icmpHdr_ = reinterpret_cast<GIcmpHdr*>(p);
      p += sizeof(GIcmpHdr); // gilgil temp 2019.05.14
      break;
    default:
      qDebug() << "unknown protocol" << uint8_t(*p);
      break;
  }
  (void)packet;
}
