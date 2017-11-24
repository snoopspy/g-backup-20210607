#include "gpcap.h"

// ----------------------------------------------------------------------------
// GPcap
// ----------------------------------------------------------------------------
GPcap::~GPcap() {
  close();
}

bool GPcap::doOpen() {
  if (!enabled_) return true;

  int dataLink = pcap_datalink(pcap_);
  qDebug() << QString("pcap_datalink return %1").arg(dataLink);
  dataLinkType_ = GPacket::intToDataLinkType(dataLink);

  if (filter_ != "") {
    if (!pcapProcessFilter(nullptr)) // gilgil temp 2016.09.30
      return false;
  }

  return this->captureThreadOpen();
}

bool GPcap::doClose() {
  if (!enabled_) return true;

  // ----- by gilgil 2009.09.01 -----
  // Strange to say, when pcap_next_ex is called after pcap_close is called, it occurs memory problem.
  // So waits until thread is terminated.
  bool res = captureThreadClose();
  if (pcap_ != nullptr) {
    pcap_close(pcap_);
    pcap_ = nullptr;
  }
  // --------------------------------
  return res;
}

GPacket::Result GPcap::read(GPacket* packet) {
  pcap_pkthdr* pkthdr;
  qCritical() << "bef pcap_next_ex"; // gilgil temp 2017.11.25
  int i = pcap_next_ex(pcap_, &pkthdr, (const u_char**)(&(packet->buf_.data_)));
  qCritical() << "aft pcap_next_ex return " << i << "state is" << (int)state_; // gilgil temp 2017.11.25
  if (state_ != Opened) return GPacket::Fail; // may be pcap_close called
  GPacket::Result res;
  switch (i) {
    case -2: // if EOF was reached reading from an offline capture
      SET_ERR(GErr::READ_FAILED, QString("pcap_next_ex return -2(%1)").arg(pcap_geterr(pcap_))); // gilgi temp 2016.09.09
      res = GPacket::Eof;
      break;
    case -1: // if an error occurred
      SET_ERR(GErr::READ_FAILED, QString("pcap_next_ex return -1(%1)").arg(pcap_geterr(pcap_)));
      res = GPacket::Fail;
      break;
    case 0 : // if a timeout occured
      res = GPacket::TimeOut;
      break;
    default: // packet captured
      packet->ts_ = pkthdr->ts;
      packet->buf_.size_ = pkthdr->caplen;
      packet->parse_ = packet->buf_;
      res = GPacket::Ok;
      break;
  }
  return res;
}

GPacket::Result GPcap::write(GPacket* packet) {
  int i = pcap_sendpacket(pcap_, packet->buf_.data_, packet->buf_.size_);
  if (i == 0) return GPacket::Ok;
  qWarning() << QString("pcap_sendpacket return %1").arg(i);
  return GPacket::Fail;
}

GPacket::Result GPcap::write(u_char* buf, size_t len) {
  int i = pcap_sendpacket(pcap_, (const u_char*)buf, (int)len);
  if (i == 0) return GPacket::Ok;
  qWarning() << QString("pcap_sendpacket return %1").arg(i);
  return GPacket::Fail;
}

GPacket::Result GPcap::relay(GPacket* packet) {
  (void)packet;
  SET_ERR(GErr::NOT_SUPPORTED, "not supported");
  return GPacket::Fail;
}

#include <netinet/in.h>
bool GPcap::pcapProcessFilter(pcap_if_t* dev) {
  u_int uNetMask;
  bpf_program code;

  if (dev != nullptr && dev->addresses != nullptr && dev->addresses->netmask != nullptr)
    uNetMask = ((struct sockaddr_in*)(dev->addresses->netmask))->sin_addr.s_addr;
  else
    uNetMask = 0xFFFFFFFF;
  if (pcap_compile(pcap_, &code, qPrintable(filter_), 1, uNetMask) < 0) {
    SET_ERR(GErr::UNKNOWN, QString("error in pcap_compile(%1)").arg(pcap_geterr(pcap_)));
    return false;
  }
  if (pcap_setfilter(pcap_, &code) < 0) {
    SET_ERR(GErr::UNKNOWN, QString("error in pcap_setfilter(%1)").arg(pcap_geterr(pcap_)));
    return false;
  }
  return true;
}
