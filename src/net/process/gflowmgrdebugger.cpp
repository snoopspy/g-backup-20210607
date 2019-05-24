#include "gflowmgrdebugger.h"
#include "net/pdu/giphdr.h"
#include <GEthPacket>
#include <GIpPacket>

// ----------------------------------------------------------------------------
// GFlowMgrDebugger
// ----------------------------------------------------------------------------
bool GFlowMgrDebugger::doOpen() {
  if (ipFlowMgr_ != nullptr) {
    ipFlowOffset_ = ipFlowMgr_->requestItems_.request("GFlowMgrTest_ip", sizeof(FlowItem));
    QObject::connect(ipFlowMgr_, &GIpFlowMgr::_flowCreated, this, &GFlowMgrDebugger::_ipFlowCreated, Qt::DirectConnection);
    QObject::connect(ipFlowMgr_, &GIpFlowMgr::_flowDeleted, this, &GFlowMgrDebugger::_ipFlowDeleted, Qt::DirectConnection);
  }

  if (tcpFlowMgr_ != nullptr) {
    tcpFlowOffset_ = tcpFlowMgr_->requestItems_.request("GFlowMgrTest_tcp", sizeof(FlowItem));
    QObject::connect(tcpFlowMgr_, &GTcpFlowMgr::_flowCreated, this, &GFlowMgrDebugger::_tcpFlowCreated, Qt::DirectConnection);
    QObject::connect(tcpFlowMgr_, &GTcpFlowMgr::_flowDeleted, this, &GFlowMgrDebugger::_tcpFlowDeleted, Qt::DirectConnection);
  }

  if (udpFlowMgr_ != nullptr) {
    udpFlowOffset_ = udpFlowMgr_->requestItems_.request("GFlowMgrTest_udp", sizeof(FlowItem));
    QObject::connect(udpFlowMgr_, &GUdpFlowMgr::_flowCreated, this, &GFlowMgrDebugger::_udpFlowCreated, Qt::DirectConnection);
    QObject::connect(udpFlowMgr_, &GUdpFlowMgr::_flowDeleted, this, &GFlowMgrDebugger::_udpFlowDeleted, Qt::DirectConnection);
  }

  return true;
}

bool GFlowMgrDebugger::doClose() {
  if (ipFlowMgr_ != nullptr) {
    QObject::disconnect(ipFlowMgr_, &GIpFlowMgr::_flowCreated, this, &GFlowMgrDebugger::_ipFlowCreated);
    QObject::disconnect(ipFlowMgr_, &GIpFlowMgr::_flowDeleted, this, &GFlowMgrDebugger::_ipFlowDeleted);
  }

  if (tcpFlowMgr_ != nullptr) {
    QObject::disconnect(tcpFlowMgr_, &GTcpFlowMgr::_flowCreated, this, &GFlowMgrDebugger::_tcpFlowCreated);
    QObject::disconnect(tcpFlowMgr_, &GTcpFlowMgr::_flowDeleted, this, &GFlowMgrDebugger::_tcpFlowDeleted);
  }

  if (udpFlowMgr_ != nullptr) {
    QObject::disconnect(udpFlowMgr_, &GUdpFlowMgr::_flowCreated, this, &GFlowMgrDebugger::_udpFlowCreated);
    QObject::disconnect(udpFlowMgr_, &GUdpFlowMgr::_flowDeleted, this, &GFlowMgrDebugger::_udpFlowDeleted);
  }

  return true;
}

void GFlowMgrDebugger::test(GPacket* packet) {
  GIpPacket* ipPacket;
  switch (packet->dataLinkType_) {
    case GPacket::Eth:
      ipPacket = PEthPacket(packet);
      break;
    case GPacket::Ip:
      ipPacket = PIpPacket(packet);
      break;
    case GPacket::Dot11:
      return;
    case GPacket::Null:
      return;
  }

  if (ipPacket->ipHdr_ != nullptr) {
    if (ipFlowMgr_ != nullptr) {
      GFlow::IpFlowKey* key = ipFlowMgr_->key_;
      FlowItem* flowItem = reinterpret_cast<FlowItem*>(ipFlowMgr_->value_->mem(ipFlowOffset_));
      flowItem->packets++;
      flowItem->bytes += packet->buf_.size_;
      qDebug() << QString("ip  size=%1 packets=%2 bytes=%3 %4>%5").
        arg(packet->buf_.size_).arg(flowItem->packets).arg(flowItem->bytes).
        arg(QString(key->sip_)).arg(QString(key->dip_)); // gilgil temp 2016.10.10
    }

    if (ipPacket->tcpHdr_ != nullptr) {
      if (tcpFlowMgr_ != nullptr) {
        GFlow::TcpFlowKey* key = tcpFlowMgr_->key_;
        FlowItem* flowItem = reinterpret_cast<FlowItem*>(tcpFlowMgr_->value_->mem(tcpFlowOffset_));
        flowItem->packets++;
        flowItem->bytes += packet->buf_.size_;
        qDebug() << QString("tcp size=%1 packets=%2 bytes=%3 %4:%5>%6:%7").
          arg(packet->buf_.size_).arg(flowItem->packets).arg(flowItem->bytes).
          arg(QString(key->sip_)).arg(key->sport_).arg(QString(key->dip_)).arg(key->dport_); // gilgil temp 2016.10.10
      }
    }

    if (ipPacket->tcpHdr_ != nullptr) {
      if (udpFlowMgr_ != nullptr) {
        GFlow::UdpFlowKey* key = udpFlowMgr_->key_;
        FlowItem* flowItem = reinterpret_cast<FlowItem*>(udpFlowMgr_->value_->mem(udpFlowOffset_));
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

void GFlowMgrDebugger::_ipFlowCreated(GPacket* packet) {
  (void)packet;
  GFlow::IpFlowKey* key = ipFlowMgr_->key_;
  GFlow::Value* value = ipFlowMgr_->value_;
  qDebug() << QString("_ipFlowCreated %1>%2").arg(QString(key->sip_), QString(key->dip_));
  FlowItem* flowItem = reinterpret_cast<FlowItem*>(value->mem(ipFlowOffset_));
  flowItem->packets = 0;
  flowItem->bytes = 0;
}

void GFlowMgrDebugger::_ipFlowDeleted(GPacket* packet) {
  (void)packet;
  GFlow::IpFlowKey* key = ipFlowMgr_->key_;
  qDebug() << QString("_ipFlowDeleted %1>%2").arg(QString(key->sip_), QString(key->dip_));
}

void GFlowMgrDebugger::_tcpFlowCreated(GPacket* packet) {
  (void)packet;
  GFlow::TcpFlowKey* key = tcpFlowMgr_->key_;
  GFlow::Value* value = tcpFlowMgr_->value_;
  qDebug() << QString("_tcpFlowCreated %1:%2>%3:%4").arg(QString(key->sip_), QString::number(key->sport_), QString(key->dip_), QString::number(key->dport_));
  FlowItem* flowItem = reinterpret_cast<FlowItem*>(value->mem(tcpFlowOffset_));
  flowItem->packets = 0;
  flowItem->bytes = 0;
}

void GFlowMgrDebugger::_tcpFlowDeleted(GPacket* packet) {
  (void)packet;
  GFlow::TcpFlowKey* key = tcpFlowMgr_->key_;
  qDebug() << QString("_tcpFlowDeleted %1:%2>%3:%4").arg(QString(key->sip_), QString::number(key->sport_), QString(key->dip_), QString::number(key->dport_));
}

void GFlowMgrDebugger::_udpFlowCreated(GPacket* packet) {
  (void)packet;
  GFlow::UdpFlowKey* key = udpFlowMgr_->key_;
  GFlow::Value* value = udpFlowMgr_->value_;
  qDebug() << QString("_udpFlowCreated %1:%2>%3:%4").arg(QString(key->sip_), QString::number(key->sport_), QString(key->dip_), QString::number(key->dport_));
  FlowItem* flowItem = reinterpret_cast<FlowItem*>(value->mem(ipFlowOffset_));
  flowItem->packets = 0;
  flowItem->bytes = 0;
}

void GFlowMgrDebugger::_udpFlowDeleted(GPacket* packet) {
  (void)packet;
  GFlow::UdpFlowKey* key = udpFlowMgr_->key_;
  qDebug() << QString("_udpFlowDeleted %1:%2>%3:%4").arg(QString(key->sip_), QString::number(key->sport_), QString(key->dip_), QString::number(key->dport_));
}