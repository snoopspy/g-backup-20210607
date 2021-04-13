#include "gconvertip.h"

// ----------------------------------------------------------------------------
// GConvertIp
// ----------------------------------------------------------------------------
void GConvertIp::convert(GPacket* packet) {
	GPacket::Dlt dlt = packet->dlt();
	switch (dlt) {
		case GPacket::Eth: {
			size_t copyLen = packet->buf_.size_ - sizeof(GEthHdr);
			memcpy(convertedIpBuf_, packet->buf_.data_ + sizeof(GEthHdr), copyLen);
			convertedIpPacket_.copyFrom(packet, GBuf(convertedIpBuf_, copyLen));
			emit converted(&convertedIpPacket_);
			break;
		}
		case GPacket::Ip: {
			emit converted(packet);
			break;
		}
		case GPacket::Dot11:
		case GPacket::Null: {
			QString msg = QString("not supported data link type(%1)").arg(GPacket::dltToString(dlt));
			SET_ERR(GErr::NOT_SUPPORTED, msg);
			return;
		}
	}
}
