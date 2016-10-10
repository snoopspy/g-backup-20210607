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
    if (elapsed >= this->ipFlowTimeout_) {
      emit _flowDeleted(&it.key(), value);
      flowMap_.erase(it);
    } else
      break;
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
    GFlow::Value* value = requestItems_.allocate();
    value->ts_ = packet->ts_;
    it = flowMap_.insert(key, value);
    emit _flowCreated(&it.key(), value);
  }

  this->key_ = (GFlow::IpFlowKey*)&it.key();
  this->value_ = it.value();
  emit processed(packet);
}
