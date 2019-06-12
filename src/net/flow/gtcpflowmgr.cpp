#include "gtcpflowmgr.h"

// ----------------------------------------------------------------------------
// GTcpFlowMgr
// ----------------------------------------------------------------------------
bool GTcpFlowMgr::doOpen() {
	flowMap_.clear();
	return GFlowMgr::doOpen();
}

bool GTcpFlowMgr::doClose() {
	for (Managable* manager: managables_) {
		for (FlowMap::iterator it = flowMap_.begin(); it != flowMap_.end(); it++) {
			GFlow::TcpFlowKey key = it.key();
			GFlow::Value* value = it.value();
			manager->tcpFlowDeleted(&key, value);
		}
	}
	flowMap_.clear();
	// requestItems_.clear(); // gilgil temp 2019.05.25
	return GFlowMgr::doClose();
}

void GTcpFlowMgr::deleteOldFlowMaps(long now) {
	FlowMap::iterator it = flowMap_.begin();
	while (it != flowMap_.end()) {
		GFlow::Value* value = it.value();
		long elapsed = now - value->ts_.tv_sec;
		long timeout = 0;
		switch (value->state_) {
			case GFlow::Value::Half: timeout = halfTimeout_; break;
			case GFlow::Value::Full: timeout = fullTimeout_; break;
			case GFlow::Value::Rst: timeout = rstTimeout_; break;
			case GFlow::Value::Fin: timeout = finTimeout_; break;
		}
		if (elapsed >= timeout) {
			//key_ = (GFlow::TcpFlowKey*)&it.key();
			key_ = const_cast<GFlow::TcpFlowKey*>(&it.key());
			value_ = value;
			for (Managable* manager: managables_)
				manager->tcpFlowDeleted(key_, value_);
			it = flowMap_.erase(it);
			continue;
		}
		it++;
	}
}

void GTcpFlowMgr::process(GPacket* packet) {
	long now = packet->ts_.tv_sec;
	if (checkInterval_ != 0 && now - lastCheckTick_ >= checkInterval_) {
		deleteOldFlowMaps(now);
		lastCheckTick_ = now;
	}

	GIpHdr* ipHdr = packet->ipHdr_;
	if (ipHdr == nullptr) return;

	GTcpHdr* tcpHdr = packet->tcpHdr_;
	if (tcpHdr == nullptr) return;

	GFlow::TcpFlowKey key{ipHdr->sip(), tcpHdr->sport(), ipHdr->dip(), tcpHdr->dport()};

	FlowMap::iterator it = flowMap_.find(key);
	if (it == flowMap_.end()) {
		GFlow::Value* value = GFlow::Value::allocate(packet->ts_, GFlow::Value::Half, requestItems_.totalMemSize_);
		it = flowMap_.insert(key, value);
		key_ = const_cast<GFlow::TcpFlowKey*>(&it.key());
		value_ = value;
		for (Managable* manager: managables_)
			manager->tcpFlowCreated(key_, value_);

		GFlow::TcpFlowKey reverseKey = GFlow::TcpFlowKey(it.key()).reverse();
		FlowMap::iterator reverseIt = flowMap_.find(reverseKey);
		if (reverseIt != flowMap_.end()) {
			it.value()->state_ = GFlow::Value::Full;
			reverseIt.value()->state_ = GFlow::Value::Full;
		}
	} else {
		GFlow::Value* value = it.value();
		value->ts_ = packet->ts_;
	}

	if ((tcpHdr->flags() & (GTcpHdr::Rst | GTcpHdr::Fin)) != 0) {
		GFlow::Value* value = it.value();
		GFlow::Value::State state = (tcpHdr->flags() & GTcpHdr::Rst) ? GFlow::Value::Rst : GFlow::Value::Fin;
		value->state_ = state;
		GFlow::TcpFlowKey reverseKey = GFlow::TcpFlowKey(it.key()).reverse();
		FlowMap::iterator reverseIt = flowMap_.find(reverseKey);
		if (reverseIt != flowMap_.end()) {
			reverseIt.value()->state_ = state;
		}
	}

	this->key_ = const_cast<GFlow::TcpFlowKey*>(&it.key());
	this->value_ = it.value();
	emit processed(packet);
}
