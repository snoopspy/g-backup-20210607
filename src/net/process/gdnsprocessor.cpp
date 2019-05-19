#include "gdnsprocessor.h"
#include <GEthPacket>
#include <GIpPacket>

// ----------------------------------------------------------------------------
// GDnsProcessor
// ----------------------------------------------------------------------------
void GDnsProcessor::process(GPacket* packet) {
  GIpPacket* ipPacket;
  switch (packet->dataLinkType_) {
    case GPacket::Eth:
      ipPacket = PEthPacket(packet);
      break;
    case GPacket::Ip:
      ipPacket = PIpPacket(packet);
      break;
    case GPacket::Dot11:
      return;
    case GPacket::Null:
      return;
  }

  GUdpHdr *udpHdr = ipPacket->udpHdr_;
  if (udpHdr == nullptr)
    return;

  if (port_ != 0) {
    if (udpHdr->sport() != port_ && udpHdr->dport() != port_)
      return;
  }

  GBuf udpData = ipPacket->udpData_;
  if (!udpData.valid())
    return;

  GDns dns;
  size_t offset = 0;
  if (!dns.decode(udpData.data_, udpData.size_, &offset)) {
    QString queryName;
    if (dns.questions_.count() != 0)
      queryName = dns.questions_.at(0).name_;
    qCritical() << "*******************************";
    qCritical() << QString("dns(%1).decode return false").arg(queryName);
    qCritical() << "*******************************";
    return;
  }

  // qDebug() << "dnsCaptured " << dns.questions.first().name; // gilgil temp 2016.10.11
  emit dnsCaptured(packet, &dns);
}
