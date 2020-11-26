#include "gtcpblock.h"
#include <cstring> // for memcpy

// ----------------------------------------------------------------------------
// GTcpBlock
// ----------------------------------------------------------------------------
GTcpBlock::GTcpBlock(QObject* parent) : GPcapDeviceWrite(parent) {
}

GTcpBlock::~GTcpBlock() {
}

bool GTcpBlock::doOpen() {
	if (!GPcapDeviceWrite::doOpen()) return false;

	if (forwardRst_ && forwardFin_) {
		SET_ERR(GErr::NOT_SUPPORTED, "Both forwardRst and forwardFin can not be true");
		return false;
	}
	if (backwardRst_ && backwardFin_) {
		SET_ERR(GErr::NOT_SUPPORTED, "Both backwardRst and backwardFin can not be true");
		return false;
	}

	forwardFinMsgStr_ = forwardFinMsg_.join("\r\n");
	backwardFinMsgStr_ = backwardFinMsg_.join("\r\n");
	maxMsgSize_ = forwardFinMsgStr_.size();
	if (maxMsgSize_ < backwardFinMsgStr_.size())
		maxMsgSize_ = backwardFinMsgStr_.size();

	return true;
}

bool GTcpBlock::doClose() {
	return GPcapDeviceWrite::doClose();
}

void GTcpBlock::block(GPacket* packet) {
	if (!enabled_) return;
	if (packet->tcpHdr_ == nullptr) return;

	GPacket::DataLinkType dataLinkType = packet->dataLinkType();
	if (dataLinkType != GPacket::Eth && dataLinkType != GPacket::Ip) {
		qWarning() << QString("not supported datalinktype(%1)").arg(GPacket::dataLinkTypeToString(dataLinkType));
		return;
	}

	bool synExist = (packet->tcpHdr_->flags() & GTcpHdr::Syn) != 0;
	bool rstExist = (packet->tcpHdr_->flags() & GTcpHdr::Rst) != 0;
	bool finExist = (packet->tcpHdr_->flags() & GTcpHdr::Fin) != 0;
	if (rstExist || finExist) return;

	GPacket* blockPacket = packet->clone(maxMsgSize_);
	GIpHdr* ipHdr = blockPacket->ipHdr_;
	GTcpHdr* tcpHdr = blockPacket->tcpHdr_;
	Q_ASSERT(ipHdr != nullptr);
	Q_ASSERT(tcpHdr != nullptr);

	size_t tcpDataSize = blockPacket->tcpData_.size_;
	uint32_t nextSeq = tcpHdr->seq();
	nextSeq += tcpDataSize + (synExist || finExist ? 1 : 0);

	if (forwardRst_ || forwardFin_) {
		// Data
		if (forwardFin_) {
			blockPacket->tcpData_.data_ = pbyte(blockPacket->tcpHdr_) + sizeof(GTcpHdr);
			memcpy(blockPacket->tcpData_.data_, qPrintable(forwardFinMsgStr_), forwardFinMsgStr_.size());
			blockPacket->tcpData_.size_ = forwardFinMsgStr_.size();
		}

		// TCP
		tcpHdr->seq_ = htonl(nextSeq);
		tcpHdr->ack_ = tcpHdr->ack_;
		tcpHdr->off_rsvd_ = (sizeof(GTcpHdr) / 4) << 4;
		tcpHdr->flags_ &= ~GTcpHdr::Syn;
		if (forwardRst_) tcpHdr->flags_ |= GTcpHdr::Rst;
		if (forwardFin_) tcpHdr->flags_ |= GTcpHdr::Fin;
		if (forwardRst_)
			tcpHdr->win_ = 0;

		// IP
		if (forwardRst_)
			ipHdr->len_ = htons(sizeof(GIpHdr) + sizeof(GTcpHdr));
		else
			ipHdr->len_ = htons(sizeof(GIpHdr) + sizeof(GTcpHdr) + forwardFinMsg_.size());
		ipHdr->tos_ = 0x44;

		// checksum
		tcpHdr->sum_ = htons(GTcpHdr::calcChecksum(ipHdr, tcpHdr));
		ipHdr->sum_ = htons(GIpHdr::calcChecksum(ipHdr));

		// ETH
		if (dataLinkType == GPacket::Eth) {
			Q_ASSERT(blockPacket->ethHdr_ != nullptr);
			blockPacket->ethHdr_->smac_ = this->intf_->mac();
		}

		// buf size
		size_t bufSize = 0;
		if (dataLinkType == GPacket::Eth) bufSize += sizeof(GEthHdr);
		bufSize += sizeof(GEthHdr) + sizeof(GIpHdr) + sizeof(GTcpHdr);
		if (forwardFin_) bufSize += forwardFinMsgStr_.size();

		// write
		write(blockPacket);
		qDebug() << "block packet sent";
	}

	emit blocked(blockPacket);
	delete blockPacket;
}
