#include "gparser.h"

// ----------------------------------------------------------------------------
// GParser
// ----------------------------------------------------------------------------
void GParser::parse(GPacket* packet) {
  (void)packet; // gilgil temp 2016.09.09
  qDebug() << "parse"; // gilgil temp 2016.09.09
}

GParser* GParser::getDefaultParser(GCapture::DataLinkType dataLinkType) {
  (void)dataLinkType; // gilgil temp 2016.09.09
  static GParser parser;
  return &parser; // gilgil temp 2016.09.09
}
