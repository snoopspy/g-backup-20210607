#include "gnetfilter.h"
#ifdef Q_OS_LINUX

#include <linux/netfilter.h> // for NF_ACCEPT
#include <libnetfilter_queue/libnetfilter_queue.h>
#include "net/parser/gparserfactory.h"

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
  if (!enabled_) return true;

  if (!autoRead_) {
    SET_ERR(GErr::NOT_SUPPORTED, "autoRead must be true");
    return false;
  }

  // opening library handle
  h_ = nfq_open();
  if (!h_) {
    SET_ERR(GErr::RETURN_NULL, "nfq_open return null");
    return false;
  }

  // unbinding existing nf_queue handler for AF_INET (if any)
  if (nfq_unbind_pf(h_, AF_INET) < 0) {
    SET_ERR(GErr::FAIL, "error during nfq_unbind_pf()");
    return false;
  }

  // binding nfnetlink_queue as nf_queue handler for AF_INET
  if (nfq_bind_pf(h_, AF_INET) < 0) {
    SET_ERR(GErr::FAIL, "error during nfq_bind_pf()");
    return false;
  }

  // binding this socket to queue
  qh_ = nfq_create_queue(h_,  uint16_t(queueNum_), &_callback, this);
  if (!qh_) {
    SET_ERR(GErr::FAIL, "error during nfq_create_queue()");
    return false;
  }

  // setting copy_packet mode
  if (nfq_set_mode(qh_, NFQNL_COPY_PACKET, 0xffff) < 0) {
    SET_ERR(GErr::FAIL, "can't set packet_copy mode");
    return false;
  }

  fd_ = nfq_fd(h_);
  qDebug() << QString("fd=%1").arg(fd_); // gilgil temp 2016.09.25

  parser_ = GParserFactory::getDefaultParser(dataLinkType());
  Q_ASSERT(parser_ != nullptr);

  return captureThreadOpen();
}

bool GNetFilter::doClose() {
  if (!enabled_) return true;

  if (fd_ != 0) {
    qDebug() << "bef call ::shutdown"; // gilgil temp 2016.09.25
    ::shutdown(fd_, SHUT_RDWR);
    qDebug() << "aft call ::shutdown"; // gilgil temp 2016.09.25

    qDebug() << "bef call ::close"; // gilgil temp 2016.09.25
    ::close(fd_);
    qDebug() << "aft call ::close"; // gilgil temp 2016.09.25
    fd_ = 0;
  }

  captureThreadClose();

  if (qh_ != nullptr) {
    qDebug() << "bef call nfq_destroy_queue"; // gilgil temp 2016.09.25
    nfq_destroy_queue(qh_);
    qDebug() << "aft call nfq_destroy_queue"; // gilgil temp 2016.09.25
    qh_ = nullptr;
  }

  if (h_ != nullptr) {
    #ifdef INSANE
    // normally, applications SHOULD NOT issue this command, since
    // it detaches other programs/sockets from AF_INET, too !
    // unbinding from AF_INET
    nfq_unbind_pf(h, AF_INET);
    #endif // INSANE

    // closing library handle
    qDebug() << "bef call nfq_close"; // gilgil temp 2016.09.25
    nfq_close(h_);
    qDebug() << "aft call nfq_close"; // gilgil temp 2016.09.25
    h_ = nullptr;
  }

  return true;
}

GPacket::Result GNetFilter::read(GPacket* packet) {
  (void)packet;
  SET_ERR(GErr::NOT_SUPPORTED, "not supported"); // gilgil temp 2019.09.25
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
  SET_ERR(GErr::NOT_SUPPORTED, "not supported"); // gilgil temp 2019.09.25
  return GPacket::Fail;
}

void GNetFilter::run() {
  qDebug() << "beg"; // gilgil temp 2016.09.27
  char* buf = static_cast<char*>(malloc(snapLen_));

  while (true) {
    //qDebug() << "bef call recv"; // gilgil temp 2016.09.27
    int res = int(recv(fd_, buf, snapLen_, 0));
    //qDebug() << "aft call recv" << res; // gilgil temp 2016.09.27
    if (res >= 0) {
      nfq_handle_packet(h_, buf, int(res));
      continue;
    } else {
      if (errno == ENOBUFS) {
        qWarning() << "losing packets!";
        continue;
      }
      qWarning() << "recv failed";
      break;
    }
  }
  free(buf);
  emit closed();
  qDebug() << "end"; // gilgil temp 2016.09.27
}

int GNetFilter::_callback(
  struct nfq_q_handle* qh,
  struct nfgenmsg* nfmsg,
  struct nfq_data* nfad,
  void* data) {

  (void)qh;
  (void)nfmsg;
  (void)nfad;

  GNetFilter* netFilter = static_cast<GNetFilter*>(data);
  GIpPacket* ipPacket = &netFilter->ipPacket;

  ipPacket->clear();
  gettimeofday(&ipPacket->ts_, nullptr);
  ipPacket->buf_.size_ = size_t(nfq_get_payload(nfad, &ipPacket->buf_.data_));
  // qDebug() << "payloadLen =" << packet.buf_.size_; // gilgil temp 2016.09.27

  if (netFilter->autoParse_) netFilter->parser_->parse(ipPacket);
  emit netFilter->captured(ipPacket);

  uint32_t id = 0;
  struct nfqnl_msg_packet_hdr* ph = nfq_get_msg_packet_hdr(nfad);
  if (ph != nullptr)
    id = ntohl(ph->packet_id);

  u_int32_t verdict;
  if (ipPacket->ctrl.block_) {
    verdict = NF_DROP;
  } else {
    verdict = u_int32_t(netFilter->acceptVerdict_);
  }

  int res = nfq_set_verdict2(qh, id, verdict, netFilter->mark_, 0, nullptr);
  return res;
}

#endif
