#include "gpcapdevicewriteip.h"

// ----------------------------------------------------------------------------
// GPcapDeviceWriteIp
// ----------------------------------------------------------------------------
bool GPcapDeviceWriteIp::doOpen() {
	if (!GPcapDeviceWrite::doOpen())
		return false;

	if (dataLinkType_ != GPacket::Ip) {
		QString msg = QString("data link layer(%1) must be GPacket::Ip").arg(GPacket::dataLinkTypeToString(dataLinkType_));
		SET_ERR(GErr::FAIL, msg);
		return false;
	}

	return true;
}

bool GPcapDeviceWriteIp::doClose() {
	return GPcapDeviceWrite::doClose();
}

GPacket::Result GPcapDeviceWriteIp::write(GPacket* packet) {
	GPacket::Result res;
	switch (packet->dataLinkType()) {
		case GPacket::Eth: {
			GBuf oldBuf = packet->buf_;
			GBuf newBuf(oldBuf.data_ + sizeof(GEthHdr), oldBuf.size_ - sizeof(GEthHdr));
			Q_ASSERT(newBuf.size_ > 0);
			packet->buf_ = newBuf;
			res = GPcapDeviceWrite::write(packet);
			packet->buf_ = oldBuf;
			break;
		}
		case GPacket::Ip: {
			res = GPcapDeviceWrite::write(packet);
			break;
		}
		case GPacket::Dot11:
		case GPacket::Null: {
			QString msg = QString("not supported data link type(%1)").arg(GPacket::dataLinkTypeToString(dataLinkType_));
			SET_ERR(GErr::NOT_SUPPORTED, msg);
			return GPacket::Fail;
		}
	}
	return res;
}
