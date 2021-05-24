#include "gnetfilterex.h"

#ifdef Q_OS_LINUX

#include <linux/netfilter.h>
#include <libnetfilter_queue/libnetfilter_queue.h>

// ----------------------------------------------------------------------------
// GNetFilterEx
// ----------------------------------------------------------------------------
GNetFilterEx::GNetFilterEx(QObject* parent) : GAsyncNetFilter(parent) {
	cb_ = &_callbackEx;
}

GNetFilterEx::~GNetFilterEx() {
	close();
}

bool GNetFilterEx::doOpen() {
	if (!enabled_) return true;

	bool res = GAsyncNetFilter::doOpen();
	if (!res) return false;

	id_ = 0;
	ipPacketEx_ = nullptr;
	return true;
}

bool GNetFilterEx::doClose() {
	if (!enabled_) return true;

	return GAsyncNetFilter::doClose();
}

#ifdef _DEBUG
static int count = 0; // gilgil temp 2021.05.21
#endif // _DEBUG

GPacket::Result GNetFilterEx::read(GPacket* packet) {
	Q_ASSERT(id_ == 0);
	Q_ASSERT(ipPacketEx_ == nullptr);

	ipPacketEx_ = PIpPacket(packet);
	// qDebug() << "bef call recv"; // gilgil temp 2021.05.22
	int res = int(::recv(fd_, recvBuf_, GPacket::MaxBufSize, 0));
	// qDebug() << "aft call recv" << res; // gilgil temp 2021.05.22
	if (res >= 0) {
		#ifdef _DEBUG
		if (++count != 1)
			qWarning() << "count is" << count;
		#endif // _DEBUG
		nfq_handle_packet(h_, recvBuf_, res);
	} else {
		if (errno == ENOBUFS) {
			qWarning() << "losing packets!";
			return GPacket::Timeout;
		}
		qWarning() << "recv failed";
		return GPacket::Fail;
	}
	return GPacket::Ok;
}

GPacket::Result GNetFilterEx::write(GBuf buf) {
	(void)buf;
	SET_ERR(GErr::NOT_SUPPORTED, "not supported");
	return GPacket::Fail;
}

GPacket::Result GNetFilterEx::write(GPacket* packet) {
	(void)packet;
	SET_ERR(GErr::NOT_SUPPORTED, "not supported");
	return GPacket::Fail;
}

GPacket::Result GNetFilterEx::relay(GPacket* packet) {
	int res;
	if (packet->ctrl.changed_)
		res = nfq_set_verdict2(qh_, id_, acceptVerdict_, mark_, packet->buf_.size_, packet->buf_.data_);
	else
		res = nfq_set_verdict2(qh_, id_, acceptVerdict_, mark_, 0, nullptr);
	id_ = 0;
	ipPacketEx_ = nullptr;
	if (res > 0)
		return GPacket::Ok;
	else {
		qWarning() << "nfq_set_verdict2 return " << res;
		return GPacket::Fail;
	}
}

GPacket::Result GNetFilterEx::drop(GPacket* packet) {
	(void)packet;
	int res = nfq_set_verdict2(qh_, id_, NF_DROP, mark_, 0, nullptr);
	id_ = 0;
	ipPacketEx_ = nullptr;
	if (res > 0)
		return GPacket::Ok;
	else {
		qWarning() << "nfq_set_verdict2 return " << res;
		return GPacket::Fail;
	}
}

void GNetFilterEx::run() {
	GCapture::run();
}

int GNetFilterEx::_callbackEx(struct nfq_q_handle* qh, struct nfgenmsg* nfmsg, struct nfq_data* nfad, void* data) {
	(void)qh;
	(void)nfmsg;

	#ifdef _DEBUG
	if (--count != 0)
		qWarning() << "count is" << count;
	#endif // _DEBUG

	GNetFilterEx* netFilterEx = static_cast<GNetFilterEx*>(data);
	Q_ASSERT(netFilterEx->qh_ == qh);
	GIpPacket* ipPacket = netFilterEx->ipPacketEx_;

	ipPacket->clear();
	gettimeofday(&ipPacket->ts_, nullptr);
	int res = nfq_get_payload(nfad, &ipPacket->buf_.data_);
	if (res == -1) {
		qCritical() << "nfq_get_payload return -1";
		return -1;
	}
	ipPacket->buf_.size_ = size_t(res);
	if (netFilterEx->autoParse_) ipPacket->parse();

	netFilterEx->id_ = 0;
	struct nfqnl_msg_packet_hdr* ph = nfq_get_msg_packet_hdr(nfad);
	if (ph != nullptr)
		netFilterEx->id_ = ntohl(ph->packet_id);

	return 0;
}

#endif
