#include "gippacket.h"

// ----------------------------------------------------------------------------
// GIpPacket
// ----------------------------------------------------------------------------
void GIpPacket::parse() {
#ifdef _DEBUG
  if (parsed_) {
    qCritical() << "already parsed";
    return;
  }
#endif // _DEBUG
  gbyte* p = buf_.data_;
  uint8_t proto;
  switch (*p & 0xF0) {
    case 0x40: // version 4
      ipHdr_ = PIpHdr(p);
      proto = ipHdr_->p();
      p += ipHdr_->hl() * 4;
      break;
    case 0x60: // version 6
      ip6Hdr_= PIp6Hdr(p);
      proto = ip6Hdr_->nh();
      p += sizeof(GIp6Hdr); // gilgil temp 2019.05.14
      break;
    default:
      qWarning() << "invalid ip header version" << uint8_t(*p); // gilgil temp 2019.05.31
      proto = 0; // unknown
      break;
  }

  switch (proto) {
    case GIpHdr::Tcp: // Tcp
      tcpHdr_ = reinterpret_cast<GTcpHdr*>(p);
      p += tcpHdr_->off() * 4;
      tcpData_ = GTcpHdr::parseData(ipHdr_, tcpHdr_);
      break;
    case GIpHdr::Udp: // Udp
      udpHdr_ = reinterpret_cast<GUdpHdr*>(p);
      p += sizeof(GUdpHdr);
      udpData_ = GUdpHdr::parseData(udpHdr_);
      break;
    case GIpHdr::Icmp: // Icmp
      icmpHdr_ = reinterpret_cast<GIcmpHdr*>(p);
      p += sizeof(GIcmpHdr); // gilgil temp 2019.05.14
      break;
    default:
      qDebug() << "unknown protocol" << proto;
      break;
  }
#ifdef _DEBUG
  parsed_ = true;
#endif // _DEBUG
}
