#include "gpacketdebugger.h"
#include <iostream>
#include "net/packet/gpacketcast.h"

QString dump(u_char* data, size_t size) {
  QString raw;
  QString hexa;
  if (size > 16) size = 16;
  while (size > 0) {
    char ch = char(*data);
    if (isprint(u_char(ch)))
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
void GPacketDebugger::enabled(GPacket* packet) {
  if (!enabled_) return;

  GEthPacket* ethPacket = GPacketCast::toEth(packet);
  GIpPacket* ipPacket = GPacketCast::toIp(packet);
  if (ipPacket == nullptr) return;

  QString msg;

  if (ethPacket != nullptr) {
    GEthHdr* ethHdr = ethPacket->ethHdr_;
    if (ethHdr != nullptr) {
      msg += QString(" eth %1>%2").arg(QString(ethHdr->smac()), QString(ethHdr->dmac()));
    }
  }

  GIpHdr* ipHdr = ipPacket->ipHdr_;
  if (ipHdr != nullptr) {
    msg += QString(" ip %1>%2").arg(QString(ipHdr->sip()), QString(ipHdr->dip()));
  }

  GTcpHdr* tcpHdr = ipPacket->tcpHdr_;
  if (tcpHdr != nullptr) {
    msg += QString(" tcp %1>%2").arg(tcpHdr->sport()).arg(tcpHdr->dport());
  }

  GBuf tcpData = ipPacket->tcpData_;
  if (tcpData.valid()) {
    msg += " " + dump(tcpData.data_, tcpData.size_);
  }

  GUdpHdr* udpHdr = ipPacket->udpHdr_;
  if (udpHdr != nullptr) {
    msg += QString(" udp %1>%2").arg(udpHdr->sport()).arg(udpHdr->dport());
  }

  GBuf udpData = ipPacket->udpData_;
  if (udpData.valid()) {
    msg += " " + dump(udpData.data_, udpData.size_);
  }

  msg = QString("%1:").arg(packet->buf_.size_) + msg;
  qDebug() << msg;

  emit debugged(packet);
}
