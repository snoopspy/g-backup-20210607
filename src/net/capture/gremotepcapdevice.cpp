#include "gremotepcapdevice.h"

// ----------------------------------------------------------------------------
// GRemotePcapDevice
// ----------------------------------------------------------------------------
GRemotePcapDevice::GRemotePcapDevice(QObject* parent) : GRemoteCapture(parent) {
}

GRemotePcapDevice::~GRemotePcapDevice() {
	close();
}

bool GRemotePcapDevice::doOpen() {
	if (!enabled_) return true;

	if (intfName_ == "") {
		SET_ERR(GErr::INTERFACE_NAME_NOT_SPECIFIED, "intfName is not specified");
		return false;
	}

	demonClient_ = new GDemonClient(std::string(qPrintable(ip_)), port_);
	GDemon::PcapOpenRep rep = demonClient_->pcapOpen(std::string(qPrintable(filter_)), std::string(qPrintable(intfName_)), snapLen_, flags_, readTimeout_, waitTimeout_, true);
	if (!rep.result_) {
		SET_ERR(GErr::FAIL, demonClient_->error_.data());
		delete demonClient_; demonClient_ = nullptr;
		return false;
	}

	intf_ = GRemoteNetInfo::instance(ip_, port_).interfaceList().findByName(intfName_);
	if (intf_ == nullptr) {
		QString msg = QString("can not find interface for %1").arg(intfName_);
		SET_ERR(GErr::VALUE_IS_NULL, msg);
		return false;
	}

	dlt_ = GPacket::intToDlt(rep.dataLink_);

	return captureThreadOpen();
}

bool GRemotePcapDevice::doClose() {
	if (demonClient_ != nullptr) {
		demonClient_->pcapClose();
		delete demonClient_;
		demonClient_ = nullptr;
	}
	return true;
}

GPacket::Result GRemotePcapDevice::read(GPacket* packet) {
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

GPacket::Result GRemotePcapDevice::write(GPacket* packet) {
	qDebug() << "fail";
	return GPacket::Fail; // gilgil temp 2021.04.22
}

GPacket::Result GRemotePcapDevice::write(GBuf buf) {
	qDebug() << "fail";
	return GPacket::Fail; // gilgil temp 2021.04.22
}

GPacket::Result GRemotePcapDevice::relay(GPacket* packet) {
	qDebug() << "fail";
	return GPacket::Fail; // gilgil temp 2021.04.22
}
