#include "gconvertethip.h"

// ----------------------------------------------------------------------------
// GConvertEthIp
// ----------------------------------------------------------------------------
bool GConvertEthIp::convert(GPacket* packet) {
	if (packet->dataLinkType_ != GPacket::Eth) {
		SET_ERR(GErr::NOT_SUPPORTED, QString("dataLinkType_(%1)) is not GPacket::Eth").arg(GPacket::dataLinkTypeToString(packet->dataLinkType_)));
		return false;
	}
	memcpy(packet->buf_.data_, packet->buf_.data_ + sizeof(GEthHdr), packet->buf_.size_ - sizeof(GEthHdr));
	emit converted(packet);
	return true;
}
