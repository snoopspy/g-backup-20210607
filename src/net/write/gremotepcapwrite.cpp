#include "gremotepcapwrite.h"

// ----------------------------------------------------------------------------
// GRemotePcapWrite
// ----------------------------------------------------------------------------
GRemotePcapWrite::GRemotePcapWrite(QObject* parent) : GRemoteWrite(parent) {
}

GRemotePcapWrite::~GRemotePcapWrite() {
	close();
}

bool GRemotePcapWrite::doClose() {
	if (demonClient_ != nullptr) {
		demonClient_->pcapClose();
		delete demonClient_;
		demonClient_ = nullptr;
	}

	return true;
}

GPacket::Result GRemotePcapWrite::write(GBuf buf) {
	GDemon::PcapWrite write;
	write.size_ = buf.size_;
	write.data_ = buf.data_;
	demonClient_->pcapWrite(write);
	return GPacket::Ok;
}

GPacket::Result GRemotePcapWrite::write(GPacket* packet) {
	GPacket::Result res = write(packet->buf_);
	if (res == GPacket::Ok)
		emit written(packet);
	return res;
}
