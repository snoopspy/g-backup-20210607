#include "gipflowmgrtest.h"

// ----------------------------------------------------------------------------
// GIpFlowMgrTest
// ----------------------------------------------------------------------------
bool GIpFlowMgrTest::doOpen() {
  if (flowMgr_ == nullptr) {
    SET_ERR(GErr::OBJECT_IS_NULL, "flowMgr is null");
    return false;
  }

  ipFlowOffset_ = flowMgr_->requestItems_.request((void*)"GIpFlowMgrTest", sizeof(FlowItem));
  QObject::connect(flowMgr_, &GIpFlowMgr::_flowCreated, this, &GIpFlowMgrTest::_flowCreated, Qt::DirectConnection);
  QObject::connect(flowMgr_, &GIpFlowMgr::_flowDeleted, this, &GIpFlowMgrTest::_flowDeleted, Qt::DirectConnection);
  return true;
}

bool GIpFlowMgrTest::doClose() {
  return true;
}

void GIpFlowMgrTest::test(GPacket* packet) {
  FlowItem* flowItem = (FlowItem*)flowMgr_->value_->mem(ipFlowOffset_);
  flowItem->packets++;
  flowItem->bytes += packet->buf_.size_;
  qDebug() << QString("size=%1 packets=%2 bytes=%3").arg(packet->buf_.size_).arg(flowItem->packets).arg(flowItem->bytes); // gilgil temp 2016.10.10
  emit tested(packet);
}

void GIpFlowMgrTest::_flowCreated(const GFlow::IpFlowKey* key, GFlow::Value* value) {
  qDebug() << "flowCreated" << qPrintable(key->sip) << qPrintable(key->dip);
  FlowItem* flowItem = (FlowItem*)value->mem(ipFlowOffset_);
  flowItem->packets = 0;
  flowItem->bytes = 0;
}

void GIpFlowMgrTest::_flowDeleted(const GFlow::IpFlowKey* key, GFlow::Value* value) {
  FlowItem* flowItem = (FlowItem*)value->mem(ipFlowOffset_);
  qDebug() << "flowDeleted" << qPrintable(key->sip) << qPrintable(key->dip) << flowItem->packets << flowItem->bytes;
}
