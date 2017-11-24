#include <QDebug>
#include "gpacket.h"
#include "net/pdu/gpdu.h"

// ----------------------------------------------------------------------------
// GPacket
// ----------------------------------------------------------------------------
int GPacket::dataLinkTypeToInt(DataLinkType dataLinkType) {
  int res = DLT_NULL;
  switch (dataLinkType) {
    case GPacket::Eth:
      res = DLT_EN10MB;
      break;
    case GPacket::Dot11:
      res = DLT_IEEE802_11_RADIO;
      break;
    case GPacket::Ip:
      res = DLT_IPV4;
      break;
    case GPacket::Null:
      res = DLT_NULL;
      break;
    default:
      res = DLT_NULL;
      break;
  }
  return res;
}

GPacket::DataLinkType GPacket::intToDataLinkType(int dataLink) {
  DataLinkType res = GPacket::Null;
  switch (dataLink) {
    case DLT_EN10MB:
      res = GPacket::Eth;
      break;
    case DLT_IEEE802_11_RADIO:
      res = GPacket::Dot11;
      break;
    case DLT_IPV4:
      res = GPacket::Ip;
      break;
    case DLT_NULL:
      res = GPacket::Null;
      break;
  }
  return res;
}

GPacket::GPacket() {
  capture_ = nullptr;
  clear();
}

GPacket::GPacket(GCapture* capture) {
  capture_ = capture;
  clear();
}

GPacket::~GPacket() {
}

void GPacket::clear() {
  bzero(&ts_, sizeof(ts_));
  bzero(&buf_, sizeof(buf_));
  bzero(&parse_, sizeof(parse_));
  control.block_ = false;
  pdus_.clear();
}
