#include "gethparser.h"
#include "net/pdu/gethpdu.h"

// ----------------------------------------------------------------------------
// GEthParser
// ----------------------------------------------------------------------------
void GEthParser::parse(GPacket* packet) {
  u_char* buf = packet->buf_;
  size_t len = packet->len_;

  GEthPdu* ethPdu = new GEthPdu(packet->buf_, packet->len_);
  packet->pdus_.push_back(ethPdu);

  packet->buf_ += ethPdu->size();
  packet->len_ -= ethPdu->size();

  GRawParser::parse(packet);

  packet->buf_ = buf;
  packet->len_ = len;
}
