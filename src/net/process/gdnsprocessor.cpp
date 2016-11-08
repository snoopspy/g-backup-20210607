#include "gdnsprocessor.h"

// ----------------------------------------------------------------------------
// GDnsProcessor
// ----------------------------------------------------------------------------
void GDnsProcessor::process(GPacket* packet) {
  GPdus& pdus = packet->pdus_;

  if (port_ != 0) {
    GUdpHdr *udpHdr = pdus.findFirst<GUdpHdr>();
    if (udpHdr == nullptr)
      return;
    if (udpHdr->sport() != port_ && udpHdr->dport() != port_)
      return;
  }

  GUdpData* udpData = pdus.findNext<GUdpData>();
  if (udpData == nullptr)
    return;

  GDns dns;
  size_t offset = 0;
  if (!dns.decode(udpData->data(), udpData->size(), &offset)) {
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
