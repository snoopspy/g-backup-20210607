#include "ghttpextract.h"

// ----------------------------------------------------------------------------
// GHttpExtract::FlowItem
// ----------------------------------------------------------------------------
GHttpExtract::FlowItem::FlowItem(GHttpExtract* httpExtract) {
	httpExtract_ = httpExtract;
	clear();
}

GHttpExtract::FlowItem::~FlowItem() {
	close();
}

void GHttpExtract::FlowItem::clear() {
	state_ = None;
	fileName_ = "";
	contentLength_ = 0;
	fp_ = nullptr;
	firstClientServerSeq_ = 0;
	firstServerClientSeq_ = 0;
}

void GHttpExtract::FlowItem::close() {
	if (fp_ != nullptr) {
		fclose(fp_);
		emit httpExtract_->finished(fileName_);
		clear();
	}
}

// ----------------------------------------------------------------------------
// GHttpExtract
// ----------------------------------------------------------------------------
bool GHttpExtract::doOpen() {
	if (tcpFlowMgr_ != nullptr) {
		tcpFlowOffset_ = tcpFlowMgr_->requestItems_.request("GFlowMgrTest_tcp", sizeof(FlowItem));
		tcpFlowMgr_->managables_.insert(this);
	}
	return true;
}

bool GHttpExtract::doClose() {
	return true;
}

void GHttpExtract::tcpFlowCreated(GFlow::TcpFlowKey* key, GFlow::Value* value) {
	(void)key;
	if (!enabled_) return;
	FlowItem* flowItem = PFlowItem(value->mem(tcpFlowOffset_));
	new (flowItem) FlowItem(this);
}

void GHttpExtract::tcpFlowDeleted(GFlow::TcpFlowKey* key, GFlow::Value* value) {
	(void)key;
	if (!enabled_) return;
	FlowItem* flowItem = PFlowItem(value->mem(tcpFlowOffset_));
	::operator delete(flowItem);
}

void GHttpExtract::write(GPacket* packet) {
	if (!enabled_) return;
	if (!packet->tcpData_.valid()) return;
	//QString tcpData(pchar(packet->tcpData_.data_), packet->tcpData_.size_);

	Q_ASSERT(tcpFlowMgr_->val_ != nullptr);
	FlowItem* flowItem = PFlowItem(tcpFlowMgr_->val_->mem(tcpFlowOffset_));

	switch (flowItem->state_) {
		case FlowItem::None:
			if (tcpData.size_ < 10) return;
			if (strcmp(pchar(tcpData.data_), "GET ") != 0) return;
			int pos = httpRequestFirstLine_.indexIn();

		case FlowItem::RequestHeaderCompleted:
		case FlowItem::ResponseHeaderCompleted:
	}

}
