#include "gpacketdebugger.h"
#include <iostream>
#include <GEthHdr>
#include <GIpHdr>
#include <GTcpData>
#include <GTcpHdr>
#include <GUdpHdr>

QString dump(u_char* data, size_t size) {
  QString raw;
  QString hexa;
  if (size > 16) size = 16;
  while (size > 0) {
    char ch = *data;
    if (isprint(ch))
      raw += ch;
    else
      raw += '.';

    char ch1 = (ch & 0xF0) >> 4;
    if (ch1 >= 10)
      ch1 += 'A' - 10;
    else
      ch1 += '0';
    char ch2 = (ch & 0x0F);
    if (ch2 >= 10)
      ch2 += 'A' - 10;
    else
      ch2 += '0';
    hexa += ch1;
    hexa += ch2;
    hexa += ' ';

    data++;
    size--;
  }
  return raw + " " + hexa;
}

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

  GTcpData* tcpData = packet->pdus_.findFirst<GTcpData>();
  if (tcpData != nullptr) {
    msg += " " + dump(tcpData->data(), tcpData->size());
  }

  GUdpHdr* udpHdr = packet->pdus_.findFirst<GUdpHdr>();
  if (udpHdr != nullptr) {
    msg += QString(" udp %1>%2").arg(udpHdr->sport()).arg(udpHdr->dport());
  }

  msg = QString("%1:").arg(packet->buf_.size_) + msg;
  std::clog << qPrintable(msg) << std::endl;

  emit debugged(packet);
}
