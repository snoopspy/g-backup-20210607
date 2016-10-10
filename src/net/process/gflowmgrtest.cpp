#include "gflowmgrtest.h"

// ----------------------------------------------------------------------------
// GFlowMgrTest
// ----------------------------------------------------------------------------
bool GFlowMgrTest::doOpen() {
  if (ipFlowMgr_ != nullptr) {
    ipFlowOffset_ = ipFlowMgr_->requestItems_.request((void*)"GFlowMgrTest_ip", sizeof(FlowItem));
    QObject::connect(ipFlowMgr_, &GIpFlowMgr::_flowCreated, this, &GFlowMgrTest::_ipFlowCreated, Qt::DirectConnection);
    QObject::connect(ipFlowMgr_, &GIpFlowMgr::_flowDeleted, this, &GFlowMgrTest::_ipFlowDeleted, Qt::DirectConnection);
  }

  if (tcpFlowMgr_ != nullptr) {
    tcpFlowOffset_ = tcpFlowMgr_->requestItems_.request((void*)"GFlowMgrTest_tcp", sizeof(FlowItem));
    QObject::connect(tcpFlowMgr_, &GTcpFlowMgr::_flowCreated, this, &GFlowMgrTest::_tcpFlowCreated, Qt::DirectConnection);
    QObject::connect(tcpFlowMgr_, &GTcpFlowMgr::_flowDeleted, this, &GFlowMgrTest::_tcpFlowDeleted, Qt::DirectConnection);
  }

  if (udpFlowMgr_ != nullptr) {
    udpFlowOffset_ = udpFlowMgr_->requestItems_.request((void*)"GFlowMgrTest_udp", sizeof(FlowItem));
    QObject::connect(udpFlowMgr_, &GUdpFlowMgr::_flowCreated, this, &GFlowMgrTest::_udpFlowCreated, Qt::DirectConnection);
    QObject::connect(udpFlowMgr_, &GUdpFlowMgr::_flowDeleted, this, &GFlowMgrTest::_udpFlowDeleted, Qt::DirectConnection);
  }

  return true;
}

bool GFlowMgrTest::doClose() {
  if (ipFlowMgr_ != nullptr) {
    QObject::disconnect(ipFlowMgr_, &GIpFlowMgr::_flowCreated, this, &GFlowMgrTest::_ipFlowCreated);
    QObject::disconnect(ipFlowMgr_, &GIpFlowMgr::_flowDeleted, this, &GFlowMgrTest::_ipFlowDeleted);
  }

  if (tcpFlowMgr_ != nullptr) {
    QObject::disconnect(tcpFlowMgr_, &GTcpFlowMgr::_flowCreated, this, &GFlowMgrTest::_tcpFlowCreated);
    QObject::disconnect(tcpFlowMgr_, &GTcpFlowMgr::_flowDeleted, this, &GFlowMgrTest::_tcpFlowDeleted);
  }

  if (udpFlowMgr_ != nullptr) {
    QObject::disconnect(udpFlowMgr_, &GUdpFlowMgr::_flowCreated, this, &GFlowMgrTest::_udpFlowCreated);
    QObject::disconnect(udpFlowMgr_, &GUdpFlowMgr::_flowDeleted, this, &GFlowMgrTest::_udpFlowDeleted);
  }

  return true;
}

void GFlowMgrTest::test(GPacket* packet) {
  if (ipFlowMgr_ != nullptr) {
    GFlow::IpFlowKey* key = ipFlowMgr_->key_;
    FlowItem* flowItem = (FlowItem*)ipFlowMgr_->value_->mem(ipFlowOffset_);
    flowItem->packets++;
    flowItem->bytes += packet->buf_.size_;
    qDebug() << QString("ip  size=%1 packets=%2 bytes=%3 %4>%5").
      arg(packet->buf_.size_).arg(flowItem->packets).arg(flowItem->bytes).
      arg(qPrintable(key->sip)).arg(qPrintable(key->dip)); // gilgil temp 2016.10.10
  }

  if (tcpFlowMgr_ != nullptr) {
    GFlow::TcpFlowKey* key = tcpFlowMgr_->key_;
    FlowItem* flowItem = (FlowItem*)tcpFlowMgr_->value_->mem(tcpFlowOffset_);
    flowItem->packets++;
    flowItem->bytes += packet->buf_.size_;
    qDebug() << QString("tcp size=%1 packets=%2 bytes=%3 %4:%5>%6:%7").
      arg(packet->buf_.size_).arg(flowItem->packets).arg(flowItem->bytes).
      arg(qPrintable(key->sip)).arg(key->sport).arg(qPrintable(key->dip)).arg(key->dport); // gilgil temp 2016.10.10
  }

  if (udpFlowMgr_ != nullptr) {
    GFlow::UdpFlowKey* key = udpFlowMgr_->key_;
    FlowItem* flowItem = (FlowItem*)udpFlowMgr_->value_->mem(udpFlowOffset_);
    flowItem->packets++;
    flowItem->bytes += packet->buf_.size_;
    qDebug() << QString("udp size=%1 packets=%2 bytes=%3 %4:%5>%6:%7").
      arg(packet->buf_.size_).arg(flowItem->packets).arg(flowItem->bytes).
      arg(qPrintable(key->sip)).arg(key->sport).arg(qPrintable(key->dip)).arg(key->dport); // gilgil temp 2016.10.10
  }

  emit tested(packet);
}

void GFlowMgrTest::_ipFlowCreated(const GFlow::IpFlowKey* key, GFlow::Value* value) {
  qDebug() << QString("_ipFlowCreated %1>%2").arg(qPrintable(key->sip), qPrintable(key->dip));
  FlowItem* flowItem = (FlowItem*)value->mem(ipFlowOffset_);
  flowItem->packets = 0;
  flowItem->bytes = 0;
}

void GFlowMgrTest::_ipFlowDeleted(const GFlow::IpFlowKey* key, GFlow::Value* value) {
  (void)value;
  qDebug() << QString("_ipFlowDeleted %1>%2").arg(qPrintable(key->sip), qPrintable(key->dip));
}

void GFlowMgrTest::_tcpFlowCreated(const GFlow::TcpFlowKey* key, GFlow::Value* value) {
  qDebug() << QString("_tcpFlowCreated %1:%2>%3:%4").arg(qPrintable(key->sip), QString::number(key->sport), qPrintable(key->dip), QString::number(key->dport));
  FlowItem* flowItem = (FlowItem*)value->mem(tcpFlowOffset_);
  flowItem->packets = 0;
  flowItem->bytes = 0;
}

void GFlowMgrTest::_tcpFlowDeleted(const GFlow::UdpFlowKey* key, GFlow::Value* value) {
  (void)value;
  qDebug() << QString("_tcpFlowDeleted %1:%2>%3:%4").arg(qPrintable(key->sip), QString::number(key->sport), qPrintable(key->dip), QString::number(key->dport));
}

void GFlowMgrTest::_udpFlowCreated(const GFlow::UdpFlowKey* key, GFlow::Value* value) {
  qDebug() << QString("_udpFlowCreated %1:%2>%3:%4").arg(qPrintable(key->sip), QString::number(key->sport), qPrintable(key->dip), QString::number(key->dport));
  FlowItem* flowItem = (FlowItem*)value->mem(ipFlowOffset_);
  flowItem->packets = 0;
  flowItem->bytes = 0;
}

void GFlowMgrTest::_udpFlowDeleted(const GFlow::UdpFlowKey* key, GFlow::Value* value) {
  (void)value;
  qDebug() << QString("_udpFlowDeleted %1:%2>%3:%4").arg(qPrintable(key->sip), QString::number(key->sport), qPrintable(key->dip), QString::number(key->dport));
}
