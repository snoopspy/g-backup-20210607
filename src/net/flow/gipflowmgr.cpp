#include "gipflowmgr.h"
#include <GEthPacket>

// ----------------------------------------------------------------------------
// GIpFlowMgr
// ----------------------------------------------------------------------------
bool GIpFlowMgr::doOpen() {
  flowMap_.clear();
  return GFlowMgr::doOpen();
}

bool GIpFlowMgr::doClose() {
  for (Managable* manager: managables_) {
    for (FlowMap::iterator it = flowMap_.begin(); it != flowMap_.end(); it++) {
      GFlow::IpFlowKey key = it.key();
      GFlow::Value* value = it.value();
      manager->ipFlowDeleted(&key, value);
    }
  }
  flowMap_.clear();
  // requestItems_.clear(); // gilgil temp 2019.05.25
  return GFlowMgr::doClose();
}

void GIpFlowMgr::deleteOldFlowMaps(GPacket* packet /* struct timeval ts */) {
  struct timeval ts = packet->ts_;
  FlowMap::iterator it = flowMap_.begin();
  while (it != flowMap_.end()) {
    GFlow::Value* value = it.value();
    long elapsed = ts.tv_sec - value->ts_.tv_sec;
    long timeout = 0;
    switch (value->state_) {
      case GFlow::Value::Half: timeout = halfTimeout_; break;
      case GFlow::Value::Full: timeout = fullTimeout_; break;
      case GFlow::Value::Rst: qCritical() << "unrecheable Rst"; timeout = 0; break;
      case GFlow::Value::Fin: qCritical() << "unrecheable Fin"; timeout = 0; break;
    }
    if (elapsed >= timeout) {
      key_ = const_cast<GFlow::IpFlowKey*>(&it.key());
      value_ = value;
      for (Managable* manager: managables_)
        manager->ipFlowDeleted(key_, value_);
      it = flowMap_.erase(it);
      continue;
    }
    it++;
  }
}

void GIpFlowMgr::process(GPacket* packet) {
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

  long now = packet->ts_.tv_sec;
  if (checkInterval_ != 0 && now - lastCheckTick_ >= checkInterval_) {
    deleteOldFlowMaps(packet);
    lastCheckTick_ = now;
  }

  GIpHdr* ipHdr = ipPacket->ipHdr_;
  if (ipHdr == nullptr) return;

  GFlow::IpFlowKey key{ipHdr->sip(), ipHdr->dip()};

  FlowMap::iterator it = flowMap_.find(key);
  if (it == flowMap_.end()) {
    GFlow::Value* value = GFlow::Value::allocate(packet->ts_, GFlow::Value::Half, requestItems_.totalMemSize_);
    it = flowMap_.insert(key, value);
    key_ = const_cast<GFlow::IpFlowKey*>(&it.key());
    value_ = value;
    for (Managable* manager: managables_)
      manager->ipFlowCreated(key_, value_);

    GFlow::IpFlowKey reverseKey = GFlow::IpFlowKey(it.key()).reverse();
    FlowMap::iterator reverseIt = flowMap_.find(reverseKey);
    if (reverseIt != flowMap_.end()) {
      it.value()->state_ = GFlow::Value::Full;
      reverseIt.value()->state_ = GFlow::Value::Full;
    }
  } else {
    GFlow::Value* value = it.value();
    value->ts_ = packet->ts_;
  }

  this->key_ = const_cast<GFlow::IpFlowKey*>(&it.key());
  this->value_ = it.value();
  emit processed(packet);
}
