#include "gipflowmgr.h"
#include "net/pdu/giphdr.h"

// ----------------------------------------------------------------------------
// GIpFlowMgr
// ----------------------------------------------------------------------------
void GIpFlowMgr::deleteOldFlowMaps(struct timeval ts) {
  FlowMap::iterator it = flowMap_.begin();
  while (it != flowMap_.end()) {
    GFlow::Value* value = it.value();
    long elapsed = ts.tv_sec - value->ts_.tv_sec;
    long timeout = value->state_ == GFlow::Value::Half ? this->halfTimeout_ : fullTimeout_;
    if (elapsed >= timeout) {
      emit _flowDeleted(&it.key(), value);
      it = flowMap_.erase(it);
      continue;
    }
    it++;
  }
}

void GIpFlowMgr::process(GPacket* packet) {
  long now = packet->ts_.tv_sec;
  if (checkInterval_ != 0 && now - lastCheckTick_ >= checkInterval_)
  {
    deleteOldFlowMaps(packet->ts_);
    lastCheckTick_ = now;
  }

  GIpHdr* ipHdr = packet->pdus_.findFirst<GIpHdr>();
  if (ipHdr == nullptr) return;

  GFlow::IpFlowKey key;
  key.sip = ipHdr->sip();
  key.dip = ipHdr->dip();

  FlowMap::iterator it = flowMap_.find(key);
  if (it == flowMap_.end()) {
    GFlow::Value* value = GFlow::Value::allocate(packet->ts_, GFlow::Value::Half, requestItems_.totalMemSize_);
    it = flowMap_.insert(key, value);
    emit _flowCreated(&it.key(), value);

    GFlow::IpFlowKey reverseKey = GFlow::IpFlowKey(it.key()).reverse();
    FlowMap::iterator reverseIt = flowMap_.find(reverseKey);
    if (reverseIt != flowMap_.end()) {
      it.value()->state_ = GFlow::Value::Full;
      reverseIt.value()->state_ = GFlow::Value::Full;
    }
  } else {
    it.value()->ts_ = packet->ts_;
  }

  this->key_ = (GFlow::IpFlowKey*)&it.key();
  this->value_ = it.value();
  emit processed(packet);
}
