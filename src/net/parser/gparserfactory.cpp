#include "gparserfactory.h"

#include "net/pdu/gdot11pdu.h"
#include "net/pdu/gethhdr.h"
#include "net/pdu/giphdr.h"
#include "net/pdu/gnullpdu.h"
#include "net/pdu/gtcpdata.h"
#include "net/pdu/gtcphdr.h"
#include "net/pdu/gudpdata.h"
#include "net/pdu/gudphdr.h"

// ----------------------------------------------------------------------------
// GParserFactory
// ----------------------------------------------------------------------------
GParserFactory::GParserFactory() {
  qRegisterMetaType<GDot11Parser*>();
  qRegisterMetaType<GEthParser*>();
  qRegisterMetaType<GNullParser*>();
  qRegisterMetaType<GIpParser*>();
  qRegisterMetaType<GTcpDataParser*>();
  qRegisterMetaType<GTcpParser*>();
  qRegisterMetaType<GUdpDataParser*>();
  qRegisterMetaType<GUdpParser*>();

  ethParser_ = new GEthParser;
  dot11Parser_ = new GDot11Parser;
  ipParser_ = new GIpParser;
  nullParser_ = new GNullParser;

  //
  // root
  //
  // ----- gilgil temp 2017.11.25 -----
  /*
  root_->addChild("GParser", "GEthParser");
  root_->addChild("GParser", "GDot11Parser");
  root_->addChild("GParser", "GIpParser");
  root_->addChild("GParser", "GNullParser");
  */
  // ----------------------------------

  //
  // L3
  //
  // root_->addChild("GEthParser", "GIpParser"); // gilgil temp 2017.11.25
  ethParser_->parserMap_[ETHERTYPE_IP] = ipParser_;

  //
  // L4
  //
  // root_->addChild("GIpParser", "GTcpParser"); // gilgil temp 2017.11.25
  // root_->addChild("GIpParser", "GUdpParser"); // gilgil temp 2017.11.25
  GTcpParser* tcpParser = new GTcpParser;
  ipParser_->parserMap_[IPPROTO_TCP] = tcpParser;
  GUdpParser* udpParser = new GUdpParser;
  ipParser_->parserMap_[IPPROTO_UDP] = udpParser;

  //
  // L7
  //
  // root_->addChild("GTcpParser", "GTcpDataParser"); // gilgil temp 2017.11.25
  // root_->addChild("GUdpParser", "GUdpDataParser"); // gilgil temp 2017.11.25
  tcpParser->parserList_.push_back(new GTcpDataParser);
  udpParser->parserList_.push_back(new GUdpDataParser);

}

GParserFactory::~GParserFactory() {
  if (ethParser_ != nullptr) {
    delete ethParser_;
    ethParser_ = nullptr;
  }
  if (ipParser_ != nullptr) {
    delete ipParser_;
    ipParser_ = nullptr;
  }
  if (ethParser_ != nullptr) {
    delete ethParser_;
    ethParser_ = nullptr;
  }
  if (ethParser_ != nullptr) {
    delete ethParser_;
    ethParser_ = nullptr;
  }

}

GParserFactory& GParserFactory::instance() {
  static GParserFactory parserFactory;
  return parserFactory;
}

void GParserFactory::init() {
  instance();
}

GParser* GParserFactory::getDefaultParser(GPacket::DataLinkType dataLinkType) {
  GParserFactory& factory = instance();
  GParser* res = nullptr;
  switch (dataLinkType) {
    case GPacket::Eth:
      res = factory.ethParser_;
      break;
    case GPacket::Dot11:
      res = factory.dot11Parser_;
      break;
    case GPacket::Ip:
      res = factory.ipParser_;
      break;
    case GPacket::Null:
      res = factory.nullParser_;
      break;
  }
  if (res == nullptr)
    qCritical() << QString("parser is null. dataLinkType=(%1)").arg((int)dataLinkType);
  return res;
}

