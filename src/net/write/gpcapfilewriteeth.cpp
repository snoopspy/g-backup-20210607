#include "gpcapfilewriteeth.h"

// ----------------------------------------------------------------------------
// GPcapFileWriteEth
// ----------------------------------------------------------------------------
bool GPcapFileWriteEth::doOpen() {
	if (!GPcapFileWrite::doOpen())
		return false;

	if (dataLinkType_ != GPacket::Eth) {
		QString msg = QString("data link layer(%1) must be GPacket::Eth").arg(GPacket::dataLinkTypeToString(dataLinkType_));
		SET_ERR(GErr::FAIL, msg);
		return false;
	}

	smac_ = smacStr_;
	dmac_ = dmacStr_;
	return true;
}

bool GPcapFileWriteEth::doClose() {
	return GPcapFileWrite::doClose();
}

GPacket::Result GPcapFileWriteEth::write(GPacket* packet) {
	GBuf oldBuf = packet->buf_;
	GBuf newBuf;
	GEthHdr* ethHdr = nullptr; // for remove warning
	switch (packet->dataLinkType()) {
		case GPacket::Eth:
			newBuf = packet->buf_;
			ethHdr = PEthHdr(oldBuf.data_);
			break;
		case GPacket::Ip:
			newBuf.data_ = temp_;
			newBuf.size_ = sizeof(GEthHdr) + oldBuf.size_;
			Q_ASSERT(newBuf.size_ <= GPacket::MaxBufSize);
			ethHdr = PEthHdr(temp_);
			memcpy(temp_ + sizeof(GEthHdr), oldBuf.data_, oldBuf.size_);
			break;
		case GPacket::Dot11:
		case GPacket::Null: {
			QString msg = QString("not supported data link type(%1)").arg(GPacket::dataLinkTypeToString(dataLinkType_));
			SET_ERR(GErr::NOT_SUPPORTED, msg);
			return GPacket::Fail;
		}
	}
	ethHdr->smac_ = smac_;
	ethHdr->dmac_ = dmac_;
	ethHdr->type_ = htons(type_);
	packet->buf_ = newBuf;
	GPacket::Result res = GPcapFileWrite::write(packet);
	packet->buf_ = oldBuf;
	return res;
}
