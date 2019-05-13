#include "gparserfactory.h"
#include "gethparser.h"
#include "gipparser.h"
#include "gdot11parser.h"
#include "gnullparser.h"

// ----------------------------------------------------------------------------
// GParserFactory
// ----------------------------------------------------------------------------
GParserFactory::GParserFactory() {
  qRegisterMetaType<GEthParser*>();
  qRegisterMetaType<GIpParser*>();
  qRegisterMetaType<GDot11Parser*>();
  qRegisterMetaType<GNullParser*>();

  ethParser_ = new GEthParser(this);
  ipParser_ = new GIpParser(this);
  dot11Parser_ = new GDot11Parser(this);
  nullParser_ = new GNullParser(this);
}

GParserFactory::~GParserFactory() {
  if (ethParser_ != nullptr) {
    delete ethParser_;
    ethParser_ = nullptr;
  }
  if (ethParser_ != nullptr) {
    delete ethParser_;
    ethParser_ = nullptr;
  }
  if (dot11Parser_ != nullptr) {
    delete dot11Parser_;
    dot11Parser_ = nullptr;
  }
  if (nullParser_ != nullptr) {
    delete nullParser_;
    nullParser_ = nullptr;
  }
}

GParser* GParserFactory::getDefaultParser(GPacket::DataLinkType dataLinkType) {
  GParserFactory& factory = instance();
  GParser* res = nullptr;
  switch (dataLinkType) {
    case GPacket::Eth:
      res = factory.ethParser_;
      break;
    case GPacket::Ip:
      res = factory.ipParser_;
      break;
    case GPacket::Dot11:
      res = factory.dot11Parser_;
      break;
    case GPacket::Null:
      res = factory.nullParser_;
      break;
  }
  if (res == nullptr)
    qCritical() << QString("parser is null. dataLinkType=(%1)").arg(int(dataLinkType));
  return res;
}

GParserFactory& GParserFactory::instance() {
  static GParserFactory parserFactory;
  return parserFactory;
}
