#include "gsyncnetfilter.h"

#ifdef Q_OS_LINUX

// ----------------------------------------------------------------------------
// GSyncNetFilter
// ----------------------------------------------------------------------------
GSyncNetFilter::GSyncNetFilter(QObject* parent) : GNetFilter(parent) {
  autoRead_ = false;
}

GSyncNetFilter::~GSyncNetFilter() {
  close();
}

bool GSyncNetFilter::doOpen() {
  qDebug() << "GSyncNetFilter::doOpen()";
  return false; // gilgil temp 2019.05.24
}

bool GSyncNetFilter::doClose() {
  qDebug() << "GSyncNetFilter::doClose()";
  return false; // gilgil temp 2019.05.24
}

GPacket::Result GSyncNetFilter::read(GPacket* packet) {
  (void)packet;
  qDebug() << "GSyncNetFilter::read()";
  return GPacket::Fail; // gilgil temp 2019.05.24
}

GPacket::Result GSyncNetFilter::write(GPacket* packet) {
  (void)packet;
  qDebug() << "GSyncNetFilter::write()";
  return GPacket::Fail; // gilgil temp 2019.05.24
}

GPacket::Result GSyncNetFilter::write(GBuf buf) {
  (void)buf;
  qDebug() << "GSyncNetFilter::write()";
  return GPacket::Fail; // gilgil temp 2019.05.24
}

GPacket::Result GSyncNetFilter::relay(GPacket* packet) {
  (void)packet;
  qDebug() << "GSyncNetFilter::relay()";
  return GPacket::Fail; // gilgil temp 2019.05.24
}

#endif
