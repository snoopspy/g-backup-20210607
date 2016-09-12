#include "gparserfactory.h"

#include "net/pdu/gethhdr.h"
#include "net/pdu/gdot11pdu.h"
#include "net/pdu/grawpdu.h"
#include "net/pdu/gnullpdu.h"

#include "net/pdu/giphdr.h"

// ----------------------------------------------------------------------------
// GParserFactory
// ----------------------------------------------------------------------------
GParserFactory::GParserFactory() {
  qRegisterMetaType<GEthParser*>();
  qRegisterMetaType<GDot11Parser*>();
  qRegisterMetaType<GRawParser*>();
  qRegisterMetaType<GNullParser*>();
  qRegisterMetaType<GIpParser*>();

  root_ = new GParser;
  root_->addChild("GParser", "GEthParser");

  //
  // root
  //
  root_->addChild("GParser", "GDot11Parser");
  root_->addChild("GParser", "GRawParser");
  root_->addChild("GParser", "GNullParser");

  //
  // L3
  //
  root_->addChild("GEthParser", "GIpParser");
}

GParserFactory::~GParserFactory() {
  if (root_ != nullptr) {
    delete root_;
    root_ = nullptr;
  }
}

GParserFactory& GParserFactory::instance() {
  static GParserFactory parserMap;
  return parserMap;
}

void GParserFactory::init() {
  instance();
}

GParser* GParserFactory::getDefaultParser(GCapture::DataLinkType dataLinkType) {
  GParserFactory& map = instance();
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
