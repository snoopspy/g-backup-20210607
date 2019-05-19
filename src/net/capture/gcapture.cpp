#include "gcapture.h"
#include "net/packet/gethpacket.h"
#include "net/packet/gippacket.h"
#include "net/packet/gdot11packet.h"
#include "net/packet/gnullpacket.h"
#include "net/parser/gparserfactory.h"

// ----------------------------------------------------------------------------
// GCaptureThread
// ----------------------------------------------------------------------------
void GCaptureThread::run() {
  GCapture* capture = dynamic_cast<GCapture*>(parent());
  Q_ASSERT(capture != nullptr);
  capture->run();
}

// ----------------------------------------------------------------------------
// GCapture
// ----------------------------------------------------------------------------
GCapture::~GCapture() {
  close();
}

bool GCapture::captureThreadOpen() {
  if (autoRead_) {
    // ----- by gilgil 2009.08.31 -----
    //
    // There can be a case that even if thread starts,
    // state remains not not Opened(still Opening) state.
    // So set state_ into Opened before open thread.
    //
    state_ = Opened;
    thread_.start();
    // --------------------------------
  }
  return true;
}

bool GCapture::captureThreadClose() {
  if (autoRead_) {
    thread_.quit();
    thread_.wait();
  }
  return true;
}

GPacket::Result GCapture::read(GPacket* packet) {
  (void)packet;
  SET_ERR(GErr::VIRTUAL_FUNCTION_CALL, "virtual function call");
  return GPacket::Fail;
}

GPacket::Result GCapture::write(GPacket* packet) {
  (void)packet;
  SET_ERR(GErr::VIRTUAL_FUNCTION_CALL, "virtual function call");
  return GPacket::Fail;
}

GPacket::Result GCapture::write(GBuf* buf) {
  (void)buf;
  SET_ERR(GErr::VIRTUAL_FUNCTION_CALL, "virtual function call");
  return GPacket::Fail;
}

GPacket::Result GCapture::relay(GPacket* packet) {
  (void)packet;
  SET_ERR(GErr::VIRTUAL_FUNCTION_CALL, "virtual function call");
  return GPacket::Fail;
}

void GCapture::run() {
  qDebug() << "beg"; // gilgil temp 2017.11.25
  GParser* parser = GParserFactory::getDefaultParser(dataLinkType());
  Q_ASSERT(parser != nullptr);

  GEthPacket ethPacket;
  GIpPacket ipPacket;
  GDot11Packet dot11Packet;
  GNullPacket nullPacket;

  GPacket* packet;
  switch(dataLinkType()) {
    case GPacket::Eth: packet = &ethPacket; break;
    case GPacket::Ip: packet = &ipPacket; break;
    case GPacket::Dot11: packet = &dot11Packet; break;
    case GPacket::Null: packet = &nullPacket; break;
  }

  while (active()) {
    GPacket::Result res = read(packet);
    if (res == GPacket::TimeOut) continue;
    if (res == GPacket::Eof || res == GPacket::Fail) break;
    if (autoParse_) parser->parse(packet);
    emit captured(packet);
    if (this->pathType() == InPath && !packet->ctrl.block_) {
      res = relay(packet);
      if (res != GPacket::Ok) {
        qWarning() << "relay return " << int(res); // gilgil temp 2015.10.29
      }
    }
  }
  qDebug() << "end"; // gilgil temp 2017.11.25
  emit closed();
}
