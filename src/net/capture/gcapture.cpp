#include "gcapture.h"
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

bool GCapture::doOpen() {
  if (autoRead_)
  {
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

bool GCapture::doClose() {
  if (autoRead_) {
    thread_.quit();
    if (!thread_.wait(G_TIMEOUT)) {
      qCritical() << QString("thread_.wait return false(%1)").arg(this->metaObject()->className());
      return false;
    }
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

GPacket::Result GCapture::write(u_char* buf, size_t len) {
  (void)buf;
  (void)len;
  SET_ERR(GErr::VIRTUAL_FUNCTION_CALL, "virtual function call");
  return GPacket::Fail;
}

GPacket::Result GCapture::relay(GPacket* packet) {
  (void)packet;
  SET_ERR(GErr::VIRTUAL_FUNCTION_CALL, "virtual function call");
  return GPacket::Fail;
}

void GCapture::run() {
  // qDebug() << "stt"; // gilgil temp 2015.10.28
  GParser* parser = GParserFactory::getDefaultParser(dataLinkType());
  Q_ASSERT(parser != nullptr);
  while (active()) {
    GPacket packet;
    packet.clear();
    packet.capture_ = this;
    GPacket::Result res = read(&packet);
    if (res == GPacket::TimeOut) continue;
    if (res == GPacket::Eof || res == GPacket::Fail) break;
    if (autoParse_) parser->parse(&packet);
    emit captured(&packet);
    if (this->pathType() == InPath) {
      res = relay(&packet);
      if (res != GPacket::Ok) {
        qWarning() << "relay return " << (int)res; // gilgil temp 2015.10.29
      }
    }
  }
  // qDebug() << "end"; // gilgil temp 2015.10.28
  emit closed();
}
