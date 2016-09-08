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
  switch (dataLink) {
    case DLT_NULL:
    case DLT_EN10MB:
    case DLT_IEEE802_11_RADIO:
      filtering = true;
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

  bool res = GCapture::doClose();

  if (pcap_ != nullptr) {
    pcap_close(pcap_);
    pcap_ = nullptr;
  }
  // --------------------------------

  return res;
}

GCapture::Result GPcap::read(GPacket* packet) {
  struct pcap_pkthdr* hdr;
  int i = pcap_next_ex(pcap_, &hdr, (const u_char**)(&(packet->buf_)));
  Result res;
  switch (i) {
    case -2: // if EOF was reached reading from an offline capture
      SET_ERR(ERROR_IN_PCAP_NEXT_EX, QString("pcap_next_ex return -2(%1)").arg(pcap_geterr(pcap_))); // gilgi temp 2016.09.09
      res = Eof;
      break;
    case -1: // if an error occurred
      SET_ERR(ERROR_IN_PCAP_NEXT_EX, QString("pcap_next_ex return -1(%1)").arg(pcap_geterr(pcap_)));
      res = Fail;
      break;
    case 0 : // if a timeout occured
      res = TimeOut;
      break;
    default: // packet captured
      packet->ts_ = hdr->ts;
      packet->len_ = hdr->caplen;
      res = Ok;
      break;
  }
  return res;
}

GCapture::Result GPcap::write(GPacket* packet) {
  //int i = pcap_sendpacket(pcap_, (const u_char*)packet->buf_, (int)packet->len_);
  int i = pcap_sendpacket(pcap_, packet->buf_, packet->len_);
  if (i == 0) return Ok;
  qWarning() << QString("pcap_sendpacket return %1").arg(i);
  return Fail;
}

GCapture::Result GPcap::write(u_char* buf, size_t len) {
  int i = pcap_sendpacket(pcap_, (const u_char*)buf, (int)len);
  if (i == 0) return Ok;
  qWarning() << QString("pcap_sendpacket return %1").arg(i);
  return Fail;
}

GCapture::Result GPcap::relay(GPacket* packet) {
  (void)packet;
  SET_ERR(GErr::NOT_SUPPORTED, "not supported");
  return Fail;
}

#include <netinet/in.h>
bool GPcap::pcapProcessFilter(pcap_if_t* dev) {
  u_int uNetMask;
  bpf_program code;

  if (dev != NULL && dev->addresses != NULL && dev->addresses->netmask != NULL)
    uNetMask = ((struct sockaddr_in*)(dev->addresses->netmask))->sin_addr.s_addr;
  else
    uNetMask = 0xFFFFFFFF;
  if (pcap_compile(pcap_, &code, qPrintable(filter_), 1, uNetMask) < 0)
  {
    SET_ERR(GErr::UNKNOWN, QString("error in pcap_compile(%1)").arg(pcap_geterr(pcap_)));
    return false;
  }
  if (pcap_setfilter(pcap_, &code) < 0)
  {
    SET_ERR(GErr::UNKNOWN, QString("error in pcap_setfilter(%1)").arg(pcap_geterr(pcap_)));
    return false;
  }
  return true;
}

