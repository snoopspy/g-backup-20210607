#include "gethpdu.h"
#include <QDebug>
#include "net/packet/gpacket.h"

// ----------------------------------------------------------------------------
// GEthPdu
// ----------------------------------------------------------------------------
GEthPdu::GEthPdu(u_char* buf, size_t len) {
  if (len > ETHER_ADDR_LEN) {
    ethHdr_ = (ETH_HDR*)buf;
  } else {
    qWarning() << QString("invalid size(%)").arg(len);
    ethHdr_ = nullptr;
  }
}

// ----------------------------------------------------------------------------
// GEthParser
// ----------------------------------------------------------------------------
GEthParser::GEthParser(QObject* parent) : GParser(parent) {
}

GEthParser::~GEthParser() { // gilgil temp 2016.09.10
}

bool GEthParser::isMatch(GPdu* prev, GPacket* packet) {
  (void)prev;
  (void)packet;
  SET_ERR(GErr::UNREACHABLE_CODE, "unreachable code");
  return false;
}

GPdu* GEthParser::doParse(GPacket* packet) {
  GEthPdu* res = new GEthPdu(packet->buf_, packet->len_);
  return res;
}
