#include "gflowmgrdebugger.h"

// ----------------------------------------------------------------------------
// GFlowMgrDebugger
// ----------------------------------------------------------------------------
bool GFlowMgrDebugger::doOpen() {
	if (ipFlowMgr_ != nullptr) {
		ipFlowOffset_ = ipFlowMgr_->requestItems_.request("GFlowMgrTest_ip", sizeof(FlowItem));
		ipFlowMgr_->managables_.insert(this);
	}

	if (tcpFlowMgr_ != nullptr) {
		tcpFlowOffset_ = tcpFlowMgr_->requestItems_.request("GFlowMgrTest_tcp", sizeof(FlowItem));
		tcpFlowMgr_->managables_.insert(this);
	}

	if (udpFlowMgr_ != nullptr) {
		udpFlowOffset_ = udpFlowMgr_->requestItems_.request("GFlowMgrTest_udp", sizeof(FlowItem));
		udpFlowMgr_->managables_.insert(this);
	}

	return true;
}

bool GFlowMgrDebugger::doClose() {
	return true;
}

void GFlowMgrDebugger::ipFlowCreated(GFlow::IpFlowKey* key, GFlow::Value* value) {
	qDebug() << QString("_ipFlowCreated %1>%2").arg(QString(key->sip_), QString(key->dip_));
	FlowItem* flowItem = PFlowItem(value->mem(ipFlowOffset_));
	flowItem->packets = 0;
	flowItem->bytes = 0;
}

void GFlowMgrDebugger::ipFlowDeleted(GFlow::IpFlowKey* key, GFlow::Value* value) {
	(void)value;
	qDebug() << QString("_ipFlowDeleted %1>%2").arg(QString(key->sip_), QString(key->dip_));
}

void GFlowMgrDebugger::tcpFlowCreated(GFlow::TcpFlowKey* key, GFlow::Value* value) {
	qDebug() << QString("_tcpFlowCreated %1:%2>%3:%4").arg(QString(key->sip_), QString::number(key->sport_), QString(key->dip_), QString::number(key->dport_));
	FlowItem* flowItem = PFlowItem(value->mem(tcpFlowOffset_));
	flowItem->packets = 0;
	flowItem->bytes = 0;
}

void GFlowMgrDebugger::tcpFlowDeleted(GFlow::TcpFlowKey* key, GFlow::Value* value) {
	(void)value;
	qDebug() << QString("_tcpFlowDeleted %1:%2>%3:%4").arg(QString(key->sip_), QString::number(key->sport_), QString(key->dip_), QString::number(key->dport_));
}

void GFlowMgrDebugger::udpFlowCreated(GFlow::UdpFlowKey* key, GFlow::Value* value) {
	qDebug() << QString("_udpFlowCreated %1:%2>%3:%4").arg(QString(key->sip_), QString::number(key->sport_), QString(key->dip_), QString::number(key->dport_));
	FlowItem* flowItem = PFlowItem(value->mem(ipFlowOffset_));
	flowItem->packets = 0;
	flowItem->bytes = 0;
}

void GFlowMgrDebugger::udpFlowDeleted(GFlow::UdpFlowKey* key, GFlow::Value* value) {
	(void)value;
	qDebug() << QString("_udpFlowDeleted %1:%2>%3:%4").arg(QString(key->sip_), QString::number(key->sport_), QString(key->dip_), QString::number(key->dport_));
}

void GFlowMgrDebugger::debug(GPacket* packet) {
	if (!enabled_) return;

	if (packet->ipHdr_ != nullptr) {
		if (ipFlowMgr_ != nullptr) {
			GFlow::IpFlowKey* key = ipFlowMgr_->key_;
			FlowItem* flowItem = PFlowItem(ipFlowMgr_->value_->mem(ipFlowOffset_));
			flowItem->packets++;
			flowItem->bytes += packet->buf_.size_;
			qDebug() << QString("ip  size=%1 packets=%2 bytes=%3 %4>%5")
				.arg(packet->buf_.size_).arg(flowItem->packets).arg(flowItem->bytes)
				.arg(QString(key->sip_)).arg(QString(key->dip_)); // gilgil temp 2016.10.10
		}

		if (packet->tcpHdr_ != nullptr) {
			if (tcpFlowMgr_ != nullptr) {
				GFlow::TcpFlowKey* key = tcpFlowMgr_->key_;
				FlowItem* flowItem = PFlowItem(tcpFlowMgr_->value_->mem(tcpFlowOffset_));
				flowItem->packets++;
				flowItem->bytes += packet->buf_.size_;
				qDebug() << QString("tcp size=%1 packets=%2 bytes=%3 %4:%5>%6:%7")
					.arg(packet->buf_.size_).arg(flowItem->packets).arg(flowItem->bytes)
					.arg(QString(key->sip_)).arg(key->sport_).arg(QString(key->dip_)).arg(key->dport_); // gilgil temp 2016.10.10
			}
		}

		if (packet->udpHdr_ != nullptr) {
			if (udpFlowMgr_ != nullptr) {
				GFlow::UdpFlowKey* key = udpFlowMgr_->key_;
				FlowItem* flowItem = PFlowItem(udpFlowMgr_->value_->mem(udpFlowOffset_));
				flowItem->packets++;
				flowItem->bytes += packet->buf_.size_;
				qDebug() << QString("udp size=%1 packets=%2 bytes=%3 %4:%5>%6:%7")
					.arg(packet->buf_.size_).arg(flowItem->packets).arg(flowItem->bytes)
					.arg(QString(key->sip_)).arg(key->sport_).arg(QString(key->dip_)).arg(key->dport_); // gilgil temp 2016.10.10
			}
		}
	}

	emit debugged(packet);
}
