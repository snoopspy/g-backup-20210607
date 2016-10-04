#include "gpacketdebugger.h"
#include <iostream>
#include <GEthHdr>
#include <GIpHdr>
#include <GTcpHdr>
#include <GUdpHdr>

// ----------------------------------------------------------------------------
// GPacketDebugger
// ----------------------------------------------------------------------------
void GPacketDebugger::debug(GPacket* packet) {
  QString msg;

  GEthHdr* ethHdr = packet->pdus_.findFirst<GEthHdr>();
  if (ethHdr != nullptr) {
    msg += QString(" eth %1>%2").arg(QString(ethHdr->smac()), QString(ethHdr->dmac()));
  }

  GIpHdr* ipHdr = packet->pdus_.findFirst<GIpHdr>();
  if (ipHdr != nullptr) {
    msg += QString(" ip %1>%2").arg(QString(ipHdr->sip()), QString(ipHdr->dip()));
  }

  GTcpHdr* tcpHdr = packet->pdus_.findFirst<GTcpHdr>();
  if (tcpHdr != nullptr) {
    msg += QString(" tcp %1>%2").arg(tcpHdr->sport()).arg(tcpHdr->dport());
  }

  GUdpHdr* udpHdr = packet->pdus_.findFirst<GUdpHdr>();
  if (udpHdr != nullptr) {
    msg += QString(" udp %1>%2").arg(udpHdr->sport()).arg(udpHdr->dport());
  }

  msg = QString("%1:").arg(packet->buf_.size_) + msg;
  std::clog << qPrintable(msg) << std::endl;

  emit debugged(packet);
}
