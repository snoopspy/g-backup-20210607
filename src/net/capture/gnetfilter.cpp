#include "gnetfilter.h"
#include <libnetfilter_queue/libnetfilter_queue.h>
#include <linux/netfilter.h> /* for NF_ACCEPT */
#include <unistd.h>
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

  if (!autoRead_) {
    SET_ERR(GErr::NOT_SUPPORTED, "autoRead must be true");
    return false;
  }

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
  qh = nfq_create_queue(h,  queueNum_, &_callback, this);
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

  parser_ = GParserFactory::getDefaultParser(dataLinkType());
  Q_ASSERT(parser_ != nullptr);

  return captureThreadOpen();
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

  captureThreadClose();

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
  char* buf = (char*)malloc(snapLen_);

  while (true) {
    qDebug() << "bef call recv"; // gilgil temp 2016.09.27
    int res = recv(fd, buf, snapLen_, 0);
    qDebug() << "aft call recv" << res; // gilgil temp 2016.09.27
    if (res >= 0) {
      nfq_handle_packet(h, buf, res);
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

  GNetFilter* netFilter = (GNetFilter*)data;
  GPacket packet;
  packet.capture_ = netFilter;

  packet.clear();
  packet.pkthdr_.caplen = (bpf_u_int32)nfq_get_payload(nfad, (unsigned char **)&packet.buf_);
  qDebug() << "payloadLen =" << packet.pkthdr_.caplen; // gilgil temp 2016.09.27

  packet.pkthdr_.len = packet.pkthdr_.caplen;
  packet.parseBuf_ = packet.buf_;
  packet.parseLen_ = (size_t)packet.pkthdr_.caplen;
  if (netFilter->autoParse_) netFilter->parser_->parse(&packet);
  emit netFilter->captured(&packet);

  int id = 0;
  struct nfqnl_msg_packet_hdr* ph = nfq_get_msg_packet_hdr(nfad);
  if (ph != nullptr)
    id = ntohl(ph->packet_id);
  int res = nfq_set_verdict(qh, id, NF_ACCEPT, 0, nullptr);
  return res;
}
