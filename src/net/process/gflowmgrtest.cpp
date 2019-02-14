#include "gflowmgrtest.h"
#include "net/pdu/giphdr.h"
#include "net/pdu/gtcphdr.h"
#include "net/pdu/gudphdr.h"

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
  GPdus& pdus = packet->pdus_;

  if (pdus.findFirst<GIpHdr>() != nullptr) {
    if (ipFlowMgr_ != nullptr) {
      GFlow::IpFlowKey* key = ipFlowMgr_->key_;
      FlowItem* flowItem = (FlowItem*)ipFlowMgr_->value_->mem(ipFlowOffset_);
      flowItem->packets++;
      flowItem->bytes += packet->buf_.size_;
      qDebug() << QString("ip  size=%1 packets=%2 bytes=%3 %4>%5").
        arg(packet->buf_.size_).arg(flowItem->packets).arg(flowItem->bytes).
        arg(QString(key->sip_)).arg(QString(key->dip_)); // gilgil temp 2016.10.10
    }

    if (pdus.findNext<GTcpHdr>() != nullptr) {
      if (tcpFlowMgr_ != nullptr) {
        GFlow::TcpFlowKey* key = tcpFlowMgr_->key_;
        FlowItem* flowItem = (FlowItem*)tcpFlowMgr_->value_->mem(tcpFlowOffset_);
        flowItem->packets++;
        flowItem->bytes += packet->buf_.size_;
        qDebug() << QString("tcp size=%1 packets=%2 bytes=%3 %4:%5>%6:%7").
          arg(packet->buf_.size_).arg(flowItem->packets).arg(flowItem->bytes).
          arg(QString(key->sip_)).arg(key->sport_).arg(QString(key->dip_)).arg(key->dport_); // gilgil temp 2016.10.10
      }
    }

    if (pdus.findNext<GUdpHdr>() != nullptr) {
      if (udpFlowMgr_ != nullptr) {
        GFlow::UdpFlowKey* key = udpFlowMgr_->key_;
        FlowItem* flowItem = (FlowItem*)udpFlowMgr_->value_->mem(udpFlowOffset_);
        flowItem->packets++;
        flowItem->bytes += packet->buf_.size_;
        qDebug() << QString("udp size=%1 packets=%2 bytes=%3 %4:%5>%6:%7").
          arg(packet->buf_.size_).arg(flowItem->packets).arg(flowItem->bytes).
          arg(QString(key->sip_)).arg(key->sport_).arg(QString(key->dip_)).arg(key->dport_); // gilgil temp 2016.10.10
      }
    }
  }

  emit tested(packet);
}

void GFlowMgrTest::_ipFlowCreated(GPacket* packet) {
  (void)packet;
  GFlow::IpFlowKey* key = ipFlowMgr_->key_;
  GFlow::Value* value = ipFlowMgr_->value_;
  qDebug() << QString("_ipFlowCreated %1>%2").arg(QString(key->sip_), QString(key->dip_));
  FlowItem* flowItem = (FlowItem*)value->mem(ipFlowOffset_);
  flowItem->packets = 0;
  flowItem->bytes = 0;
}

void GFlowMgrTest::_ipFlowDeleted(GPacket* packet) {
  (void)packet;
  GFlow::IpFlowKey* key = ipFlowMgr_->key_;
  qDebug() << QString("_ipFlowDeleted %1>%2").arg(QString(key->sip_), QString(key->dip_));
}

void GFlowMgrTest::_tcpFlowCreated(GPacket* packet) {
  (void)packet;
  GFlow::TcpFlowKey* key = tcpFlowMgr_->key_;
  GFlow::Value* value = tcpFlowMgr_->value_;
  qDebug() << QString("_tcpFlowCreated %1:%2>%3:%4").arg(QString(key->sip_), QString::number(key->sport_), QString(key->dip_), QString::number(key->dport_));
  FlowItem* flowItem = (FlowItem*)value->mem(tcpFlowOffset_);
  flowItem->packets = 0;
  flowItem->bytes = 0;
}

void GFlowMgrTest::_tcpFlowDeleted(GPacket* packet) {
  (void)packet;
  GFlow::TcpFlowKey* key = udpFlowMgr_->key_;
  qDebug() << QString("_tcpFlowDeleted %1:%2>%3:%4").arg(QString(key->sip_), QString::number(key->sport_), QString(key->dip_), QString::number(key->dport_));
}

void GFlowMgrTest::_udpFlowCreated(GPacket* packet) {
  (void)packet;
  GFlow::UdpFlowKey* key = udpFlowMgr_->key_;
  GFlow::Value* value = udpFlowMgr_->value_;
  qDebug() << QString("_udpFlowCreated %1:%2>%3:%4").arg(QString(key->sip_), QString::number(key->sport_), QString(key->dip_), QString::number(key->dport_));
  FlowItem* flowItem = (FlowItem*)value->mem(ipFlowOffset_);
  flowItem->packets = 0;
  flowItem->bytes = 0;
}

void GFlowMgrTest::_udpFlowDeleted(GPacket* packet) {
  (void)packet;
  GFlow::UdpFlowKey* key = udpFlowMgr_->key_;
  qDebug() << QString("_udpFlowDeleted %1:%2>%3:%4").arg(QString(key->sip_), QString::number(key->sport_), QString(key->dip_), QString::number(key->dport_));
}
