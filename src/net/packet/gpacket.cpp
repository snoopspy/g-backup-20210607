#include "gpacket.h"

// ----------------------------------------------------------------------------
// GPacket
// ----------------------------------------------------------------------------
QString GPacket::dataLinkTypeToString(DataLinkType dataLinkType) {
	QString res;
	switch (dataLinkType) {
		case GPacket::Eth:
			res = "GPacket::Eth";
			break;
		case GPacket::Ip:
			res = "GPacket::Ip";
			break;
		case GPacket::Dot11:
			res = "GPacket::Dot11";
			break;
		case GPacket::Null:
			res = "GPacket::Null";
			break;
	}
	return res;
}

int GPacket::dataLinkTypeToInt(DataLinkType dataLinkType) {
	int res;
	switch (dataLinkType) {
		case GPacket::Eth:
			res = DLT_EN10MB;
			break;
		case GPacket::Ip:
			res = DLT_RAW;
			break;
		case GPacket::Dot11:
			res = DLT_IEEE802_11_RADIO;
			break;
		case GPacket::Null:
		default:
			res = DLT_NULL;
			break;
	}
	return res;
}

GPacket::DataLinkType GPacket::intToDataLinkType(int dataLink) {
	DataLinkType res;
	switch (dataLink) {
		case DLT_EN10MB:
			res = GPacket::Eth;
			break;
		case DLT_RAW:
		case DLT_IPV4:
		case DLT_IPV6:
			res = GPacket::Ip;
			break;
		case DLT_IEEE802_11_RADIO:
			res = GPacket::Dot11;
			break;
		case DLT_NULL:
		default:
			res = GPacket::Null;
			break;
	}
	return res;
}

// ----- gilgil temp 2019.05.31 -----
/*
 * GBuf GPacket::getWriteBuf(GBuf buf, DataLinkType src, DataLinkType dst) {
	GBuf res = buf;
	if (src != dst) {
		if (src == GPacket::Eth && dst == GPacket::Ip) {
			res.data_ += sizeof(GEthHdr);
			res.size_ -= sizeof(GEthHdr);
		} else {
			QString msg = QString("invalid data link type src:%1 dst:%2").arg(int(src)).arg(int(dst));
			qCritical() << msg;
			return GBuf(nullptr, 0);
		}
	}
	return res;
}
*/
// ----------------------------------

GPacket::operator QString() const {
	QString smac;
	QString dmac;
	GIp sip(0);
	GIp dip(0);
	QString protoStr = "unknown";
	uint16_t sport = 0;
	uint16_t dport = 0;

	if (ethHdr_ != nullptr) {
		smac = QString(ethHdr_->smac());
		dmac = QString(ethHdr_->dmac());
	}

	if (ipHdr_ != nullptr) {
		sip = ipHdr_->sip();
		dip = ipHdr_->dip();
		uint8_t proto = ipHdr_->p();
		if (proto == GIpHdr::Tcp) {
			protoStr = "TCP";
			Q_ASSERT(tcpHdr_ != nullptr);
			sport = tcpHdr_->sport();
			dport = tcpHdr_->dport();
		}
		if (proto == GIpHdr::Udp) {
			protoStr = "UDP";
			Q_ASSERT(udpHdr_ != nullptr);
			sport = udpHdr_->sport();
			dport = udpHdr_->dport();
		}
	}

	QString msg = QString("%1 %2:%3 > %4:%5")
		.arg(protoStr)
		.arg(QString(sip)).arg(QString::number(sport))
		.arg(QString(dip)).arg(QString::number(dport));

	if (smac != "")
		msg = QString("%1 > %2 %3").arg(smac).arg(dmac).arg(msg);

	return msg;
}

void GPacket::parse() {
	qCritical() << "virtual function call";
}

GPacket* GPacket::clone(size_t extra) {
	(void)extra;
	qCritical() << "virtual function call";
	return nullptr;
}

void GPacket::doClone(GPacket* source, size_t extra) {
	clear();
	ts_ = source->ts_;
	size_t size = source->buf_.size_;
	buf_.data_ = pbyte(malloc(size + extra));
	memcpy(buf_.data_, source->buf_.data_, size);
	buf_.size_ = size;
	bufSelfAlloc_ = true;

	ctrl = source->ctrl;

	parse();
}
