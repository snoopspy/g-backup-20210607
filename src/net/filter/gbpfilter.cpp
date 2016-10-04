#include "gbpfilter.h"

// ----------------------------------------------------------------------------
// GBpFilter
// ----------------------------------------------------------------------------
GBpFilter::GBpFilter(QObject* parent): GStateObj(parent) {
}

GBpFilter::~GBpFilter() {
  close();
}

bool GBpFilter::doOpen() {
  int res;

  int dataLink = GPacket::dataLinkTypeToInt(dataLinkType_);
  pcap_ = pcap_open_dead(dataLink, 1);
  if (pcap_ == nullptr) {
    SET_ERR(GErr::FAIL, "error in pcap_open_dead return null");
    return false;
  }

  code_ = (bpf_program*)malloc(sizeof(bpf_program));
  res = pcap_compile(pcap_, code_, qPrintable(filter_), 1, 0xFFFFFFFF);
  if (res < 0) {
    SET_ERR(GErr::FAIL, QString("error in pcap_compile(%1)").arg(pcap_geterr(pcap_)));
    return false;
  }

  return true;
}

bool GBpFilter::doClose() {
  if (pcap_ != nullptr) {
    pcap_close(pcap_);
    pcap_ = nullptr;
  }

  if (code_ != nullptr) {
    pcap_freecode(code_);
    free(code_);
    code_ = nullptr;
  }

  return true;
}

void GBpFilter::check(GPacket* packet) {
  u_int len = packet->pkthdr_.caplen;
  Q_ASSERT(code_ != nullptr);
  int res = bpf_filter(code_->bf_insns, (const u_char*)packet->buf_, len, len);
  if (res > 0)
    emit filtered(packet);
  else
    emit notFiltered(packet);
}
