#include "gpacket.h"

// ----------------------------------------------------------------------------
// GPacket
// ----------------------------------------------------------------------------
int GPacket::dataLinkTypeToInt(DataLinkType dataLinkType) {
  int res;
  switch (dataLinkType) {
    case GPacket::Eth:
      res = DLT_EN10MB;
      break;
    case GPacket::Ip:
      res = DLT_IPV4;
      break;
    case GPacket::Dot11:
      res = DLT_IEEE802_11_RADIO;
      break;
    case GPacket::Null:
      res = DLT_NULL;
      break;
  }
  return res;
}

GPacket::DataLinkType GPacket::intToDataLinkType(int dataLink) {
  DataLinkType res;
  switch (dataLink) {
    case DLT_EN10MB:
      res = GPacket::Eth;
      break;
    case DLT_IPV4:
      res = GPacket::Ip;
      break;
    case DLT_IEEE802_11_RADIO:
      res = GPacket::Dot11;
      break;
    case DLT_NULL:
      res = GPacket::Null;
      break;
    default:
      res = GPacket::Null;
      break;
  }
  return res;
}

#include <QDebug> // gilgil temp 2019.05.25
void GPacket::parse() {
  qDebug() << "GPacket::parse()"; // gilgil temp 2019.05.25
}
