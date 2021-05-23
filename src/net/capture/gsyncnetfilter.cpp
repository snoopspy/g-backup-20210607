#include "gsyncnetfilter.h"

#ifdef Q_OS_LINUX

// ----------------------------------------------------------------------------
// GSyncNetFilter
// ----------------------------------------------------------------------------
GSyncNetFilter::GSyncNetFilter(QObject* parent) : GAsyncNetFilter(parent) {
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
	SET_ERR(GErr::NOT_SUPPORTED, "not supported");
	return GPacket::Fail;
}

GPacket::Result GSyncNetFilter::write(GBuf buf) {
	(void)buf;
	SET_ERR(GErr::NOT_SUPPORTED, "not supported");
	return GPacket::Fail;
}

GPacket::Result GSyncNetFilter::write(GPacket* packet) {
	(void)packet;
	SET_ERR(GErr::NOT_SUPPORTED, "not supported");
	return GPacket::Fail;
}

GPacket::Result GSyncNetFilter::relay(GPacket* packet) {
	(void)packet;
	SET_ERR(GErr::NOT_SUPPORTED, "not supported");
	return GPacket::Fail;
}

#endif
