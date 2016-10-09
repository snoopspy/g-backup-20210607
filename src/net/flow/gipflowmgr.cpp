#include "gipflowmgr.h"

// ----------------------------------------------------------------------------
// GIpFlowMgr
// ----------------------------------------------------------------------------
void GIpFlowMgr::deleteOldFlowMaps(struct timeval ts) {
  for (FlowMap::iterator it = flowMap_.begin(); it != flowMap_.end(); it++) {
    GFlow::Value& value = it.value();
    long elapsed = ts.tv_sec - value.ts_.tv_sec;
    if (elapsed >= this->ipFlowTimeout_) {
      GFlow::IpFlowKey& key = (GFlow::IpFlowKey&)it.key();
      emit deleted(&key, &value);
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
  emit processed(packet);
}
