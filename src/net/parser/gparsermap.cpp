#include "gparsermap.h"
#include "net/pdu/gethpdu.h"
#include "net/pdu/gdot11pdu.h"
#include "net/pdu/grawpdu.h"
#include "net/pdu/gnullpdu.h"

// ----------------------------------------------------------------------------
// GParserMap
// ----------------------------------------------------------------------------
GParserMap::GParserMap() {
  root_ = new GParser;
  root_->addChild("GParser", "GEthParser");
  root_->addChild("GEthParser", "GNullParser");
  //root_->addChild("GParser", "GDot11Parser");
  //root_->addChild("GParser", "GRawParser");
  //root_->addChild("GParser", "GNullParser");
}

GParserMap::~GParserMap() {
  if (root_ != nullptr) {
    delete root_;
    root_ = nullptr;
  }
}

GParserMap& GParserMap::instance() {
  static GParserMap parserMap;
  return parserMap;
}

GParser* GParserMap::getDefaultParser(GCapture::DataLinkType dataLinkType) {
  GParserMap& map = instance();
  GParser* res = nullptr;
  switch (dataLinkType) {
    case GCapture::Eth:
      res = map.root_->findFirstChild("GEthParser");
      break;
    case GCapture::Dot11:
      res = map.root_->findFirstChild("GDot11Parser");
      break;
    case GCapture::Raw:
      res = map.root_->findFirstChild("GRawParser");
      break;
    case GCapture::Null:
      res = map.root_->findFirstChild("GNullParser");
      break;
  }
  if (res == nullptr)
    qCritical() << QString("parser is null. dataLinkType=(%1)").arg((int)dataLinkType);
  return res;
}
