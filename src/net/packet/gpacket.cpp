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
      res = DLT_RAW;
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
    case DLT_RAW:
    case DLT_IPV4:
    case DLT_IPV6:
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

GBuf GPacket::getWriteBuf(GBuf buf, DataLinkType src, DataLinkType dst) {
  GBuf res = buf;
  if (src != dst) {
    if (src == GPacket::Eth && dst == GPacket::Ip) {
      buf.data_ += sizeof(GEthHdr);
      buf.size_ -= sizeof(GEthHdr);
    } else {
      QString msg = QString("invalid data link type src:%1 dst:%2").arg(int(src)).arg(int(dst));
      qCritical() << msg;
      return GBuf(nullptr, 0);
    }
  }
  return res;
}

void GPacket::parse() {
  qCritical() << "virtual function call";
}
