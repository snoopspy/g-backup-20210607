#include "gparser.h"
#include "gnullparser.h"
#include "grawparser.h"
#include "gethparser.h"
#include "gdot11parser.h"

// ----------------------------------------------------------------------------
// GParser
// ----------------------------------------------------------------------------
void GParser::parse(GPacket* packet) {
  (void)packet; // gilgil temp 2016.09.09
  qDebug() << "parse"; // gilgil temp 2016.09.09
}

GParser* GParser::getDefaultParser(GCapture::DataLinkType dataLinkType) {
  switch (dataLinkType) {
    case GCapture::Null:
      return &GNullParser::instance();
    case GCapture::Raw:
      return &GRawParser::instance();
    case GCapture::Eth:
      return &GEthParser::instance();
    case GCapture::Dot11:
      return &GDot11Parser::instance();
  }
}
