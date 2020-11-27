#include "gtcpblock.h"
#include <cstring> // for memcpy

// ----------------------------------------------------------------------------
// GTcpBlock
// ----------------------------------------------------------------------------
GTcpBlock::GTcpBlock(QObject* parent) : GPcapDeviceWrite(parent) {
}

GTcpBlock::~GTcpBlock() {
	close();
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

	return true;
}

bool GTcpBlock::doClose() {
	return GPcapDeviceWrite::doClose();
}

void GTcpBlock::sendBlockPacket(GPacket* packet, GTcpBlock::Direction direction, GTcpBlock::BlockType blockType, uint32_t seq, uint32_t ack, QString msg) {
	GPacket* blockPacket = nullptr;
	if (msg != "") {
		blockPacket = packet->clone(msg.size());
		packet = blockPacket;
	}

	GTcpHdr* tcpHdr = packet->tcpHdr_;
	Q_ASSERT(tcpHdr != nullptr);

	//
	// Data
	//
	if (blockType == Fin) {
		packet->tcpData_.data_ = pbyte(tcpHdr) + sizeof(GTcpHdr);
		memcpy(packet->tcpData_.data_, qPrintable(msg), msg.size());
		packet->tcpData_.size_ = forwardFinMsgStr_.size();
	}

	//
	// TCP
	//
	if (direction == Backward)
		std::swap(tcpHdr->sport_, tcpHdr->dport_);
	tcpHdr->seq_ = htonl(seq);
	tcpHdr->ack_ = htonl(ack);
	if (blockType == Rst) {
		tcpHdr->flags_ = GTcpHdr::Rst | GTcpHdr::Ack;
		tcpHdr->win_ = 0;
	} else {
		tcpHdr->flags_ |= GTcpHdr::Fin | GTcpHdr::Ack | GTcpHdr::Psh;
	}
	tcpHdr->off_rsvd_ = (sizeof(GTcpHdr) / 4) << 4;
	tcpHdr->flags_ &= ~GTcpHdr::Syn;

	//
	// IP
	//
	GIpHdr* ipHdr = packet->ipHdr_;
	Q_ASSERT(ipHdr != nullptr);
	if (blockType == Rst)
		ipHdr->len_ = htons(sizeof(GIpHdr) + sizeof(GTcpHdr));
	else
		ipHdr->len_ = htons(sizeof(GIpHdr) + sizeof(GTcpHdr) + msg.size());
	ipHdr->tos_ = 0x44;
	if (direction == Backward) {
		ipHdr->ttl_ = 0x80;
		std::swap(ipHdr->sip_, ipHdr->dip_);
	}

	// checksum
	tcpHdr->sum_ = htons(GTcpHdr::calcChecksum(ipHdr, tcpHdr));
	ipHdr->sum_ = htons(GIpHdr::calcChecksum(ipHdr));

	// ETH
	GPacket::DataLinkType dataLinkType = packet->dataLinkType();
	if (dataLinkType == GPacket::Eth) {
		GEthHdr* ethHdr = packet->ethHdr_;
		Q_ASSERT(ethHdr != nullptr);
		if (direction == Forward) {
			ethHdr->smac_ = this->intf_->mac();
		} else {
			ethHdr->dmac_ = ethHdr->smac();
			ethHdr->smac_ = this->intf_->mac();
		}
	}

	// buf size
	size_t bufSize = 0;
	if (dataLinkType == GPacket::Eth) bufSize += sizeof(GEthHdr);
	bufSize += sizeof(GIpHdr) + sizeof(GTcpHdr);
	if (blockType == Fin) bufSize += msg.size();
	packet->buf_.size_ = bufSize;

	// write
	write(packet);
	if (blockPacket != nullptr) delete blockPacket;
	qDebug() << "block packet sent";
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
	bool ackExist = (packet->tcpHdr_->flags() & GTcpHdr::Ack) != 0;
	if (rstExist || finExist) return;

	GTcpHdr* tcpHdr = packet->tcpHdr_;
	Q_ASSERT(tcpHdr != nullptr);

	size_t tcpDataSize = packet->tcpData_.size_;
	uint32_t seq = tcpHdr->seq();
	uint32_t nextSeq = seq + tcpDataSize + (synExist || finExist ? 1 : 0);
	uint32_t ack = tcpHdr->ack();

	bool _blocked = false;
	if (forwardRst_) {
		if (synExist && !ackExist)
			sendBlockPacket(packet, Forward, Rst, seq, 0); // useless
		else
			sendBlockPacket(packet, Forward, Rst, nextSeq, ack);
		_blocked = true;
	}
	if (forwardFin_) {
		if (!synExist) {
			// sendBlockPacket(blockPacket, Forward, Fin, seq, ack, forwardFinMsgStr_); // gilgil temp 2020.11.26
			sendBlockPacket(packet, Forward, Fin, nextSeq, ack, forwardFinMsgStr_);
			_blocked = true;
		}
	}
	if (backwardRst_) {
		if (synExist && !ackExist)
			sendBlockPacket(packet, Backward, Rst, 0, nextSeq);
		else
			sendBlockPacket(packet, Backward, Rst, ack, nextSeq);
		_blocked = true;
	}
	if (backwardFin_) {
		if (!synExist) {
			sendBlockPacket(packet, Backward, Fin, ack, nextSeq, backwardFinMsgStr_);
			_blocked = true;
		}
	}

	if (_blocked)
		emit blocked(packet);
}
