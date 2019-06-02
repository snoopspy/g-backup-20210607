#include "gbypasssslblock.h"

// ----------------------------------------------------------------------------
// GBypassSslBlock
// ----------------------------------------------------------------------------
bool GBypassSslBlock::doOpen() {
  if (tcpFlowMgr_ != nullptr) {
    tcpFlowOffset_ = tcpFlowMgr_->requestItems_.request("GFlowMgrTest_tcp", sizeof(FlowItem));
    tcpFlowMgr_->managables_.insert(this);
  }

  return true;
}

bool GBypassSslBlock::doClose() {
  return true;
}

void GBypassSslBlock::tcpFlowCreated(GFlow::TcpFlowKey* key, GFlow::Value* value) {
  qDebug() << QString("_tcpFlowCreated %1:%2>%3:%4").arg(QString(key->sip_), QString::number(key->sport_), QString(key->dip_), QString::number(key->dport_));
  FlowItem* flowItem = PFlowItem(value->mem(tcpFlowOffset_));
  flowItem->processed_ = false;
}

void GBypassSslBlock::tcpFlowDeleted(GFlow::TcpFlowKey* key, GFlow::Value* value) {
  (void)value;
  qDebug() << QString("_tcpFlowDeleted %1:%2>%3:%4").arg(QString(key->sip_), QString::number(key->sport_), QString(key->dip_), QString::number(key->dport_));
}

void GBypassSslBlock::bypass(GPacket* packet) {
  GIpHdr* ipHdr = packet->ipHdr_;
  if (ipHdr == nullptr) return;
  GTcpHdr* tcpHdr = packet->tcpHdr_;
  if (tcpHdr == nullptr) return;

  GFlow::TcpFlowKey* key = tcpFlowMgr_->key_;
  FlowItem* flowItem = PFlowItem(tcpFlowMgr_->value_->mem(tcpFlowOffset_));
  if (flowItem->processed_) return;

  GBuf tcpData = packet->tcpData_;
  if (!tcpData.valid()) return; // no tcp data

  flowItem->processed_ = true;
  if (tcpData.size_ <= 16) return; // too small data

  if (tcpData.data_[0] != 0x16) return;

  qDebug() << QString("tcp size=%1 %2:%3>%4:%5").
    arg(packet->buf_.size_).
    arg(QString(key->sip_)).arg(key->sport_).arg(QString(key->dip_)).arg(key->dport_); // gilgil temp 2016.10.10

  size_t orgDataSize = tcpData.size_;
  size_t firstDataSize = 16;
  size_t secondDataSize = orgDataSize - firstDataSize;

  static const int SIZE = 65536;
  gbyte temp[65536];
  memcpy(temp, tcpData.data_, tcpData.size_);

  //
  // first 16 bytes
  //
  ipHdr->len_ = htons(sizeof(GIpHdr) + tcpHdr->off() * 4  + firstDataSize);
  tcpHdr->sum_ = htons(GTcpHdr::calcChecksum(ipHdr, tcpHdr));
  ipHdr->sum_ = htons(GIpHdr::calcChecksum(ipHdr));
  packet->buf_.size_ = ipHdr->len();
  emit bypassed(packet);

  // ----- gilgil temp 2019.06.03 -----
  /*
  //
  // second extra bytes
  //
  ipHdr->len_ = htons(sizeof(GIpHdr) + tcpHdr->off() * 4  + secondDataSize);
  memcpy(tcpData.data_, temp + firstDataSize, secondDataSize);
  tcpHdr->seq_ = htonl(tcpHdr->seq() + 16);
  tcpHdr->sum_ = htons(GTcpHdr::calcChecksum(ipHdr, tcpHdr));
  ipHdr->sum_ = htons(GIpHdr::calcChecksum(ipHdr));
  packet->buf_.size_ = ipHdr->len();
  writer_->write(packet);
  */
  // ----------------------------------

  packet->ctrl.block_ = true;
}
