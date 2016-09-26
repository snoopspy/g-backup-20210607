#include "gnetfilter.h"
#include <libnetfilter_queue/libnetfilter_queue.h>
#include <linux/netfilter.h> /* for NF_ACCEPT */
#include <unistd.h>

// ----------------------------------------------------------------------------
// GNetFilter
// ----------------------------------------------------------------------------
GNetFilter::GNetFilter(QObject* parent) : GCapture(parent) {
  qDebug() << "GNetFilter::GNetFilter"; // gilgil temp 2016.09.25
}

GNetFilter::~GNetFilter()  {
  qDebug() << "GNetFilter::~GNetFilter"; // gilgil temp 2016.09.25
  close();
}

bool GNetFilter::doOpen() {
  qDebug() << "GNetFilter::doOpen"; // gilgil temp 2016.09.25

  // opening library handle
  h = nfq_open();
  if (!h) {
    SET_ERR(GErr::RETURN_NULL, "nfq_open return null");
    return false;
  }

  // unbinding existing nf_queue handler for AF_INET (if any)
  if (nfq_unbind_pf(h, AF_INET) < 0) {
    SET_ERR(GErr::FAIL, "error during nfq_unbind_pf()");
    return false;
  }

  // binding nfnetlink_queue as nf_queue handler for AF_INET
  if (nfq_bind_pf(h, AF_INET) < 0) {
    SET_ERR(GErr::FAIL, "error during nfq_bind_pf()");
    return false;
  }

  // binding this socket to queue
  qh = nfq_create_queue(h,  num_, &_callback, this);
  if (!qh) {
    SET_ERR(GErr::FAIL, "error during nfq_create_queue()");
    return false;
  }

  // setting copy_packet mode
  if (nfq_set_mode(qh, NFQNL_COPY_PACKET, 0xffff) < 0) {
    SET_ERR(GErr::FAIL, "can't set packet_copy mode");
    return false;
  }

  fd = nfq_fd(h);
  qDebug() << QString("fd=%1").arg(fd); // gilgil temp 2016.09.25

  Q_ASSERT(buffer_ == nullptr);
  buffer_ = (u_char*)malloc(snapLen_);
  Q_ASSERT(buffer_ != nullptr);

  return GCapture::doOpen(); // start thread
}

bool GNetFilter::doClose() {
  if (fd != 0) {
    qDebug() << "bef call ::shutdown"; // gilgil temp 2016.09.25
    ::shutdown(fd, SHUT_RDWR);
    qDebug() << "aft call ::shutdown"; // gilgil temp 2016.09.25

    qDebug() << "bef call ::close"; // gilgil temp 2016.09.25
    ::close(fd);
    qDebug() << "aft call ::close"; // gilgil temp 2016.09.25
    fd = 0;
  }

  GCapture::doClose(); // wait thread

  if (qh != nullptr) {
    qDebug() << "bef call nfq_destroy_queue"; // gilgil temp 2016.09.25
    nfq_destroy_queue(qh);
    qDebug() << "aft call nfq_destroy_queue"; // gilgil temp 2016.09.25
    qh = nullptr;
  }

  if (h != nullptr) {
    #ifdef INSANE
    // normally, applications SHOULD NOT issue this command, since
    // it detaches other programs/sockets from AF_INET, too !
    // unbinding from AF_INET
    nfq_unbind_pf(h, AF_INET);
    #endif // INSANE

    // closing library handle
    qDebug() << "bef call nfq_close"; // gilgil temp 2016.09.25
    nfq_close(h);
    qDebug() << "aft call nfq_close"; // gilgil temp 2016.09.25
    h = nullptr;
  }

  if (buffer_ != nullptr) {
    free(buffer_);
    buffer_ = nullptr;
  }

  return true;
}

GPacket::Result GNetFilter::read(GPacket* packet) {
  qDebug() << "bef call recv";
  int res = recv(fd, buffer_, snapLen_, 0);
  qDebug() << "aft call recv" << res;
  if (res >= 0) {
    Q_ASSERT(res >= skipLen_);
    this->len_ = res;
    packet->buf_ = buffer_ + skipLen_;
    res -= skipLen_;
    // TODO packet->pkthdr_.ts
    packet->pkthdr_.caplen = (bpf_u_int32)res;
    packet->pkthdr_.len = (bpf_u_int32)res;
    packet->parseBuf_ = packet->buf_;
    packet->parseLen_ = (size_t)res;
    return GPacket::Ok;
  }
  if (errno == ENOBUFS) {
    qWarning() << QString("losing packets! res=%1").arg(res);
    return GPacket::TimeOut;
  }

  qWarning() << QString("recv return %1").arg(res);
  return GPacket::Fail;
}

GPacket::Result GNetFilter::write(GPacket* packet) {
  (void)packet;
  SET_ERR(GErr::NOT_SUPPORTED, "not supported"); // gilgil temp 2019.09.25
  return GPacket::Fail;
}

GPacket::Result GNetFilter::write(u_char* buf, size_t len) {
  (void)buf;
  (void)len;
  SET_ERR(GErr::NOT_SUPPORTED, "not supported"); // gilgil temp 2019.09.25
  return GPacket::Fail;
}

GPacket::Result GNetFilter::relay(GPacket* packet) {
  (void)packet;
  int res = nfq_handle_packet(h, (char*)buffer_, (int)len_);
  if (res != 0) {
    SET_ERR(GErr::FAIL, QString("nfq_handle_packet return %1 len=%2").arg(res).arg(len_));
    return GPacket::Fail;
  }
  return GPacket::Ok;
}

int GNetFilter::_callback(
  struct nfq_q_handle* qh,
  struct nfgenmsg* nfmsg,
  struct nfq_data* nfa,
  void* data) {
  (void)nfmsg;

  // qDebug() << "beg _callback"; // gilgil temp 2016.09.26
  int id = 0;
  struct nfqnl_msg_packet_hdr* ph = nfq_get_msg_packet_hdr(nfa);
  if (ph != nullptr)
    id = ntohl(ph->packet_id);

  unsigned char* payload;
  int payloadLen = nfq_get_payload(nfa, &payload);
  qDebug() << "payloadLen=" << payloadLen;
  GNetFilter* netFilter = (GNetFilter*)data;
  u_int32_t verdictLen = (u_int32_t)netFilter->len_ - netFilter->skipLen_;
  unsigned char* verdictData = netFilter->buffer_ + netFilter->skipLen_;
  //int res = nfq_set_verdict(qh, id, NF_ACCEPT, verdictLen, verdictData);
  int res = nfq_set_verdict(qh, id, NF_ACCEPT, 0, nullptr);
  // qDebug() << "end _callback" << res;  // gilgil temp 2016.09.26
  return res;
}
