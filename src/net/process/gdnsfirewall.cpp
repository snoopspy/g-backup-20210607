#include "gdnsfirewall.h"
#include <GEthPacket>
#include <GIpPacket>

// ----------------------------------------------------------------------------
// GDnsFirewall
// ----------------------------------------------------------------------------
GDnsFirewall::GDnsFirewall(QObject* parent) : GFlowMgr(parent) {
  exceptionIpList_
      << "127.0.0.1"
      << "224.0.0.1"
      << "224.0.0.255"
      << "255.255.255.255";
}

bool GDnsFirewall::doOpen() {
    if (dnsProcessor_ != nullptr) {
    QObject::connect(dnsProcessor_, &GDnsProcessor::dnsCaptured, this, &GDnsFirewall::_dnsProcess, Qt::DirectConnection);
  }

  if (ipFlowMgr_ != nullptr) {
    QObject::connect(ipFlowMgr_, &GIpFlowMgr::_flowCreated, this, &GDnsFirewall::_ipFlowCreated, Qt::DirectConnection);
    QObject::connect(ipFlowMgr_, &GIpFlowMgr::_flowDeleted, this, &GDnsFirewall::_ipFlowDeleted, Qt::DirectConnection);
  }

  if (tcpFlowMgr_ != nullptr) {
    QObject::connect(tcpFlowMgr_, &GTcpFlowMgr::_flowCreated, this, &GDnsFirewall::_tcpFlowCreated, Qt::DirectConnection);
    QObject::connect(tcpFlowMgr_, &GTcpFlowMgr::_flowDeleted, this, &GDnsFirewall::_tcpFlowDeleted, Qt::DirectConnection);
  }

  if (udpFlowMgr_ != nullptr) {
    QObject::connect(udpFlowMgr_, &GUdpFlowMgr::_flowCreated, this, &GDnsFirewall::_udpFlowCreated, Qt::DirectConnection);
    QObject::connect(udpFlowMgr_, &GUdpFlowMgr::_flowDeleted, this, &GDnsFirewall::_udpFlowDeleted, Qt::DirectConnection);
  }

  exceptionIps_.clear();
  foreach (QString exceptionIp, exceptionIpList_) {
    if (exceptionIp == "") continue;
    GIp ip = exceptionIp;
    exceptionIps_.insert(ip);
  }

  dnsMap_.clear();
  ipBlockMap_.clear();
  tcpBlockMap_.clear();
  udpBlockMap_.clear();

  return true;
}

bool GDnsFirewall::doClose() {
  if (dnsProcessor_ != nullptr) {
    QObject::disconnect(dnsProcessor_, &GDnsProcessor::dnsCaptured, this, &GDnsFirewall::_dnsProcess);
  }

  if (ipFlowMgr_ != nullptr) {
    QObject::disconnect(ipFlowMgr_, &GIpFlowMgr::_flowCreated, this, &GDnsFirewall::_ipFlowCreated);
    QObject::disconnect(ipFlowMgr_, &GIpFlowMgr::_flowDeleted, this, &GDnsFirewall::_ipFlowDeleted);
  }

  if (tcpFlowMgr_ != nullptr) {
    QObject::disconnect(tcpFlowMgr_, &GTcpFlowMgr::_flowCreated, this, &GDnsFirewall::_tcpFlowCreated);
    QObject::disconnect(tcpFlowMgr_, &GTcpFlowMgr::_flowDeleted, this, &GDnsFirewall::_tcpFlowDeleted);
  }

  if (udpFlowMgr_ != nullptr) {
    QObject::disconnect(udpFlowMgr_, &GUdpFlowMgr::_flowCreated, this, &GDnsFirewall::_udpFlowCreated);
    QObject::disconnect(udpFlowMgr_, &GUdpFlowMgr::_flowDeleted, this, &GDnsFirewall::_udpFlowDeleted);
  }

  return true;
}

void GDnsFirewall::deleteOldDnsMaps(GPacket* packet /* struct timeval ts */) {
  struct timeval ts = packet->ts_;
  DnsMap::iterator it = dnsMap_.begin();
  while (it != dnsMap_.end()) {
    DnsItems& items = it.value();

    DnsItems::iterator it2 = items.begin();
    while (it2 != items.end()) {
      DnsItem& item = *it2;
      long elapsed = ts.tv_sec - item.ts_.tv_sec;
      if (elapsed >= item.ttl_ + extraTtlTime_) {
        it2 = items.erase(it2);
        continue;
      }
      it2++;
    }

    if (items.empty()) {
      it = dnsMap_.erase(it);
      continue;
    }
    it++;
  }
}

bool GDnsFirewall::checkBlockFromDnsMap(GFlow::IpFlowKey ipFlowKey, struct timeval ts) {
  bool block = true;
  DnsMap::iterator it = dnsMap_.find(ipFlowKey);
  if (it != dnsMap_.end()) {
    DnsItems& items = it.value();
    foreach (const DnsItem& item, items) {
      if (ts.tv_sec <= item.ts_.tv_sec + item.ttl_ + extraTtlTime_) {
        block = false;
        break;
      }
    }
  } else {
    it = dnsMap_.find(ipFlowKey.reverse());
    if (it != dnsMap_.end()) {
      DnsItems& items = it.value();
      foreach (const DnsItem& item, items) {
        if (ts.tv_sec <= item.ts_.tv_sec + item.ttl_ + extraTtlTime_) {
          block = false;
          break;
        }
      }
    }
  }
  return block;
}


void GDnsFirewall::debugPacket(QString msg, GPacket* packet) {
  GIpPacket* ipPacket;
  switch (packet->dataLinkType_) {
    case GPacket::Eth:
      ipPacket = static_cast<GEthPacket*>(packet);
      break;
    case GPacket::Ip:
      ipPacket = static_cast<GIpPacket*>(packet);
      break;
    case GPacket::Dot11:
      return;
    case GPacket::Null:
      return;
  }

  QString proto = "unknown";
  GIp sip, dip;
  uint16_t sport = 0, dport = 0;

  GIpHdr* ipHdr = ipPacket->ipHdr_;
  if (ipHdr != nullptr) {
    proto = "ip";
    sip = ipHdr->sip();
    dip = ipHdr->dip();

    GTcpHdr* tcpHdr = ipPacket->tcpHdr_;
    if (tcpHdr != nullptr) {
      proto = "tcp";
      sport = tcpHdr->sport();
      dport = tcpHdr->dport();
    }

    GUdpHdr* udpHdr = ipPacket->udpHdr_;
    if (udpHdr != nullptr) {
      proto = "udp";
      sport = udpHdr->sport();
      dport = udpHdr->dport();
    }
  }

  if (sport == 0 && dport == 0)
    qDebug() << QString("%1 %2 %3>%4").arg(msg, proto, QString(sip), QString(dip));
  else
    qDebug() << QString("%1 %2 %3:%4>:%5:%6").arg(msg, proto, QString(sip), QString::number(sport), QString(dip), QString::number(dport));
}

void GDnsFirewall::check(GPacket* packet) {
  GIpPacket* ipPacket;
  switch (packet->dataLinkType_) {
    case GPacket::Eth:
      ipPacket = static_cast<GEthPacket*>(packet);
      break;
    case GPacket::Ip:
      ipPacket = static_cast<GIpPacket*>(packet);
      break;
    case GPacket::Dot11:
      return;
    case GPacket::Null:
      return;
  }

  GIpHdr* ipHdr = ipPacket->ipHdr_;
  if (ipHdr == nullptr) {
    packet->ctrl.block_ = false;
    emit notBlocked(packet);
    return;
  }

  bool decided = false;
  bool block = false;

  GTcpHdr* tcpHdr;
  GUdpHdr* udpHdr;
  if ((tcpHdr = ipPacket->tcpHdr_) != nullptr) {
    //
    // TCP
    //
    GFlow::TcpFlowKey tcpFlowKey{ipHdr->sip(), tcpHdr->sport(), ipHdr->dip(), tcpHdr->dport()};
    TcpBlockMap::iterator it = tcpBlockMap_.find(tcpFlowKey);
    if (it != tcpBlockMap_.end()) {
      decided = true;
      block = it.value();
    } else {
      it = tcpBlockMap_.find(tcpFlowKey.reverse());
      if (it != tcpBlockMap_.end()) {
        block = it.value();
      } else {
        debugPacket("check can not find tcp", packet);
        decided = true;
        block = true;
      }
    }
  }

  if (!decided && (udpHdr = ipPacket->udpHdr_) != nullptr) {
    //
    // UDP
    //
    if (udpHdr->sport() == 53 || udpHdr->dport() == 53) {
      // debugPacket("check dns", packet); // gilgil temp 2016.11.08
      decided = true;
      block = false;
    } else {
      GFlow::UdpFlowKey udpFlowKey{ipHdr->sip(), udpHdr->sport(), ipHdr->dip(), udpHdr->dport()};
      UdpBlockMap::iterator it = udpBlockMap_.find(udpFlowKey);
      if (it != udpBlockMap_.end()) {
        decided = true;
        block = it.value();
      } else {
        it = udpBlockMap_.find(udpFlowKey.reverse());
        if (it != udpBlockMap_.end()) {
          decided = true;
          block = it.value();
        } else {
          debugPacket("check can not find udp", packet);
          decided = true;
          block = true;
        }
      }
    }
  }

  if (!decided) {
    //
    // Other
    //
    GFlow::IpFlowKey ipFlowKey{ipHdr->sip(), ipHdr->dip()};
    IpBlockMap::iterator it = ipBlockMap_.find(ipFlowKey);
    if (it != ipBlockMap_.end()) {
      block = it.value();
    } else {
      it = ipBlockMap_.find(ipFlowKey.reverse());
      if (it != ipBlockMap_.end()) {
        block = it.value();
      } else {
        debugPacket("check can not find ip", packet);
        block = true;
      }
    }
  }

  packet->ctrl.block_ = block;
  if (block) {
    debugPacket("check block", packet);
    emit blocked(packet);
  } else {
    // debugPacket("check pass ", packet); // gilgil temp 2016.11.08
    emit notBlocked(packet);
  }
}

void GDnsFirewall::_ipFlowCreated(GPacket* packet) {
  GFlow::IpFlowKey ipFlowKey = *ipFlowMgr_->key_;
  bool block;
  if (exceptionIps_.find(ipFlowKey.sip_) != exceptionIps_.end() || exceptionIps_.find(ipFlowKey.dip_) != exceptionIps_.end())
    block = false;
  else
    block = checkBlockFromDnsMap(ipFlowKey, packet->ts_);
  ipBlockMap_[ipFlowKey] = block;
  // debugPacket("_ipFlowCreated " + QString(block ? "block" : "pass "), packet); // gilgil temp 2016.11.08
}

void GDnsFirewall::_ipFlowDeleted(GPacket* packet) {
  (void)packet;
  GFlow::IpFlowKey* key = ipFlowMgr_->key_;
  ipBlockMap_.remove(*key);
  // debugPacket("_ipFlowDeleted", packet); // gilgil temp 2016.11.08
}

void GDnsFirewall::_tcpFlowCreated(GPacket* packet) {
  GFlow::TcpFlowKey* tcpFlowKey = tcpFlowMgr_->key_;
  GFlow::IpFlowKey ipFlowKey{tcpFlowKey->sip_, tcpFlowKey->dip_};
  bool block;
  if (exceptionIps_.find(ipFlowKey.sip_) != exceptionIps_.end() || exceptionIps_.find(ipFlowKey.dip_) != exceptionIps_.end())
    block = false;
  else
    block = checkBlockFromDnsMap(ipFlowKey, packet->ts_);
  tcpBlockMap_[*tcpFlowKey] = block;
  // debugPacket("_tcpFlowCreated " + QString(block ? "block" : "pass "), packet); // gilgil temp 2016.11.08
}

void GDnsFirewall::_tcpFlowDeleted(GPacket* packet) {
  (void)packet;
  GFlow::TcpFlowKey* tcpFlowKey = tcpFlowMgr_->key_;
  tcpBlockMap_.remove(*tcpFlowKey);
  // debugPacket("_tcpFlowDeleted", packet); // gilgil temp 2016.11.08
}

void GDnsFirewall::_udpFlowCreated(GPacket* packet) {
  GFlow::UdpFlowKey* udpFlowKey = udpFlowMgr_->key_;
  GFlow::IpFlowKey ipFlowKey{udpFlowKey->sip_, udpFlowKey->dip_};
  bool block;
  if (exceptionIps_.find(ipFlowKey.sip_) != exceptionIps_.end() || exceptionIps_.find(ipFlowKey.dip_) != exceptionIps_.end())
    block = false;
  else
    block = checkBlockFromDnsMap(ipFlowKey, packet->ts_);
  udpBlockMap_[*udpFlowKey] = block;
  // debugPacket("_udpFlowCreated " + QString(block ? "block" : "pass "), packet); // gilgil temp 2016.11.08
}

void GDnsFirewall::_udpFlowDeleted(GPacket* packet) {
  (void)packet;
  GFlow::UdpFlowKey* udpFlowKey = udpFlowMgr_->key_;
  udpBlockMap_.remove(*udpFlowKey);
  // debugPacket("_udpFlowDeleted", packet); // gilgil temp 2016.11.08
}

void GDnsFirewall::_dnsProcess(GPacket* packet, GDns* dns) {
  if (dns->answers_.count() == 0) return;

  GIpPacket* ipPacket;
  switch (packet->dataLinkType_) {
    case GPacket::Eth:
      ipPacket = static_cast<GEthPacket*>(packet);
      break;
    case GPacket::Ip:
      ipPacket = static_cast<GIpPacket*>(packet);
      break;
    case GPacket::Dot11:
      return;
    case GPacket::Null:
      return;
  }

  GIpHdr* ipHdr = ipPacket->ipHdr_;
  Q_ASSERT(ipHdr != nullptr);
  GFlow::IpFlowKey ipFlowKey(ipHdr->dip(), uint32_t(0)); // ipHdr-dip() is dns query client ip

  foreach (GDns::ResourceRecord answer, dns->answers_) {
    if (answer.type_ != 0x0001) // Host Address
      continue;
    GIp ip = ntohl(*(uint32_t*)answer.data_.data());
    ipFlowKey.dip_ = ip; // dns response ip(server ip)
    qDebug() << QString("name=%1 ttl=%2 data=%3 %4").arg(answer.name_).arg(answer.ttl_).arg(QString(ip), QString::number(ip, 16));

    DnsItem newItem;
    newItem.name_ = answer.name_;
    newItem.ttl_ = answer.ttl_;
    newItem.ts_ = packet->ts_;

    DnsItems& items = dnsMap_[ipFlowKey];
    foreach (const DnsItem& item, items) {
      if (item.name_ == newItem.name_) {
        (DnsItem&)item = newItem;
        return;
      }
    }
    items.append(newItem);
  }
}
