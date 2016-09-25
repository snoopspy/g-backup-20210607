#include "gpcap.h"

// ----------------------------------------------------------------------------
// GPcap
// ----------------------------------------------------------------------------
GPcap::~GPcap() {
  close();
}

bool GPcap::doOpen() {
  bool filtering = false;
  int dataLink = pcap_datalink(pcap_);
  qDebug() << QString("pcap_datalink return %1").arg(dataLink);
  switch (dataLink) {
    case DLT_EN10MB:
      dataLinkType_ = GPacket::Eth;
      filtering = true;
      break;
    case DLT_IEEE802_11_RADIO:
      dataLinkType_ = GPacket::Dot11;
      filtering = true;
      break;
    case DLT_IPV4:
      dataLinkType_ = GPacket::Ipv4;
      filtering = true;
      break;
    case DLT_NULL:
      dataLinkType_ = GPacket::Null;
      filtering = true;
      break;
  }

  if (filtering && filter_ != "") {
    if (!pcapProcessFilter(nullptr)) // gilgil temp 2015.10.28
      return false;
  }

  return GCapture::doOpen();
}

bool GPcap::doClose() {
  // ----- by gilgil 2009.09.01 -----
  // Strange to say, when pcap_next_ex is called after pcap_close is called, it occurs memory problem.
  // So waits until thread is terminated.
  bool res = GCapture::doClose(); // wait thread
  if (pcap_ != nullptr) {
    pcap_close(pcap_);
    pcap_ = nullptr;
  }
  // --------------------------------
  return res;
}

GPacket::Result GPcap::read(GPacket* packet) {
  pcap_pkthdr* pkthdr;
  // qDebug() << "bef pcap_next_ex"; // gilgil temp 2016.09.25
  int i = pcap_next_ex(pcap_, &pkthdr, (const u_char**)(&(packet->buf_)));
  // qDebug() << "aft pcap_next_ex return " << i << "state is" << (int)state_; // gilgil temp 2016.09.25
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
      packet->pkthdr_ = *pkthdr;
      packet->parseBuf_ = packet->buf_;
      packet->parseLen_ = (size_t)pkthdr->caplen;
      res = GPacket::Ok;
      break;
  }
  return res;
}

GPacket::Result GPcap::write(GPacket* packet) {
  int i = pcap_sendpacket(pcap_, packet->buf_, packet->pkthdr_.caplen);
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
