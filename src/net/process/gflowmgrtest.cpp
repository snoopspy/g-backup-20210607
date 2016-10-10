#include "gflowmgrtest.h"

// ----------------------------------------------------------------------------
// GFlowMgrTest
// ----------------------------------------------------------------------------
bool GFlowMgrTest::doOpen() {
  if (ipFlowMgr_ == nullptr) {
    SET_ERR(GErr::OBJECT_IS_NULL, "flowMgr is null");
    return false;
  }

  ipFlowOffset_ = ipFlowMgr_->requestItems_.request((void*)"GFlowMgrTest_ip", sizeof(FlowItem));
  QObject::connect(ipFlowMgr_, &GIpFlowMgr::_flowCreated, this, &GFlowMgrTest::_ipFlowCreated, Qt::DirectConnection);
  QObject::connect(ipFlowMgr_, &GIpFlowMgr::_flowDeleted, this, &GFlowMgrTest::_ipFlowDeleted, Qt::DirectConnection);

  udpFlowOffset_ = udpFlowMgr_->requestItems_.request((void*)"GFlowMgrTest_udp", sizeof(FlowItem));
  QObject::connect(udpFlowMgr_, &GUdpFlowMgr::_flowCreated, this, &GFlowMgrTest::_udpFlowCreated, Qt::DirectConnection);
  QObject::connect(udpFlowMgr_, &GUdpFlowMgr::_flowDeleted, this, &GFlowMgrTest::_udpFlowDeleted, Qt::DirectConnection);

  return true;
}

bool GFlowMgrTest::doClose() {
  QObject::disconnect(ipFlowMgr_, &GIpFlowMgr::_flowCreated, this, &GFlowMgrTest::_ipFlowCreated);
  QObject::disconnect(ipFlowMgr_, &GIpFlowMgr::_flowDeleted, this, &GFlowMgrTest::_ipFlowDeleted);

  QObject::disconnect(udpFlowMgr_, &GUdpFlowMgr::_flowCreated, this, &GFlowMgrTest::_udpFlowCreated);
  QObject::disconnect(udpFlowMgr_, &GUdpFlowMgr::_flowDeleted, this, &GFlowMgrTest::_udpFlowDeleted);

  return true;
}

void GFlowMgrTest::test(GPacket* packet) {
  FlowItem* flowItem = (FlowItem*)ipFlowMgr_->value_->mem(ipFlowOffset_);
  flowItem->packets++;
  flowItem->bytes += packet->buf_.size_;
  qDebug() << QString("size=%1 packets=%2 bytes=%3").arg(packet->buf_.size_).arg(flowItem->packets).arg(flowItem->bytes); // gilgil temp 2016.10.10
  emit tested(packet);
}

void GFlowMgrTest::_ipFlowCreated(const GFlow::IpFlowKey* key, GFlow::Value* value) {
  qDebug() << "_ipFlowCreated" << qPrintable(key->sip) << qPrintable(key->dip);
  FlowItem* flowItem = (FlowItem*)value->mem(ipFlowOffset_);
  flowItem->packets = 0;
  flowItem->bytes = 0;
}

void GFlowMgrTest::_ipFlowDeleted(const GFlow::IpFlowKey* key, GFlow::Value* value) {
  FlowItem* flowItem = (FlowItem*)value->mem(ipFlowOffset_);
  qDebug() << "_ipFlowDeleted" << qPrintable(key->sip) << qPrintable(key->dip) << flowItem->packets << flowItem->bytes;
}

void GFlowMgrTest::_udpFlowCreated(const GFlow::UdpFlowKey* key, GFlow::Value* value) {
  qDebug() << "_udpFlowCreated" << qPrintable(key->sip) << qPrintable(key->dip);
  FlowItem* flowItem = (FlowItem*)value->mem(ipFlowOffset_);
  flowItem->packets = 0;
  flowItem->bytes = 0;
}

void GFlowMgrTest::_udpFlowDeleted(const GFlow::UdpFlowKey* key, GFlow::Value* value) {
  FlowItem* flowItem = (FlowItem*)value->mem(ipFlowOffset_);
  qDebug() << "_udpFlowDeleted" << qPrintable(key->sip) << qPrintable(key->dip) << flowItem->packets << flowItem->bytes;
}
