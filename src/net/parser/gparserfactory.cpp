#include "gparserfactory.h"

#include "net/pdu/gdot11pdu.h"
#include "net/pdu/gethhdr.h"
#include "net/pdu/giphdr.h"
#include "net/pdu/gnullpdu.h"
#include "net/pdu/grawpdu.h"
#include "net/pdu/gtcphdr.h"

#include "net/pdu/giphdr.h"

// ----------------------------------------------------------------------------
// GParserFactory
// ----------------------------------------------------------------------------
GParserFactory::GParserFactory() {
  qRegisterMetaType<GDot11Parser*>();
  qRegisterMetaType<GEthParser*>();
  qRegisterMetaType<GNullParser*>();
  qRegisterMetaType<GRawParser*>();
  qRegisterMetaType<GIpParser*>();
  qRegisterMetaType<GTcpParser*>();

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

  //
  // L4
  //
  root_->addChild("GIpParser", "GTcpParser");
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

GParser* GParserFactory::getDefaultParser(GPacket::DataLinkType dataLinkType) {
  GParserFactory& map = instance();
  GParser* res = nullptr;
  switch (dataLinkType) {
    case GPacket::Eth:
      res = map.root_->findFirstChild("GEthParser");
      break;
    case GPacket::Dot11:
      res = map.root_->findFirstChild("GDot11Parser");
      break;
    case GPacket::Raw:
      res = map.root_->findFirstChild("GRawParser");
      break;
    case GPacket::Null:
      res = map.root_->findFirstChild("GNullParser");
      break;
  }
  if (res == nullptr)
    qCritical() << QString("parser is null. dataLinkType=(%1)").arg((int)dataLinkType);
  return res;
}
