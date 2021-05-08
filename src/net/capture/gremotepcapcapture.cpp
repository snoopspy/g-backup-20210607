#include "gremotepcapcapture.h"

// ----------------------------------------------------------------------------
// GRemotePcapCapture
// ----------------------------------------------------------------------------
bool GRemotePcapCapture::doOpen() {
	captureThreadOpen();
	return true;
}

bool GRemotePcapCapture::doClose() {
	if (demonClient_ != nullptr)
		demonClient_->pcapClose();

	captureThreadClose();

	if (demonClient_ != nullptr) {
		delete demonClient_;
		demonClient_ = nullptr;
	}

	return true;
}

GPacket::Result GRemotePcapCapture::read(GPacket* packet) {
	packet->clear();

	GDemon::PcapRead read = demonClient_->pcapRead();
	if (read.data_ == nullptr) {
		SET_ERR(GErr::READ_FAILED, "read fail");
		return GPacket::Fail;
	}

	packet->ts_ = read.pktHdr_.ts;
	packet->buf_.data_ = read.data_;
	packet->buf_.size_ = read.pktHdr_.caplen;
	if (autoParse_) packet->parse();
	return GPacket::Ok;
}

GPacket::Result GRemotePcapCapture::write(GBuf buf) {
	GDemon::PcapWrite write;
	write.size_ = buf.size_;
	write.data_ = buf.data_;
	demonClient_->pcapWrite(write);
	return GPacket::Ok;
}

GPacket::Result GRemotePcapCapture::write(GPacket* packet) {
	return write(packet->buf_);
}

GPacket::Result GRemotePcapCapture::relay(GPacket* packet) {
	(void)packet;
	SET_ERR(GErr::NOT_SUPPORTED, "not supported");
	return GPacket::Fail;
}


