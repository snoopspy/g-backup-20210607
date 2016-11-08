#include "gdnsfirewall.h"
#include "net/pdu/giphdr.h"
#include "net/pdu/gtcphdr.h"
#include "net/pdu/gudphdr.h"

// ----------------------------------------------------------------------------
// GDnsFirewall
// ----------------------------------------------------------------------------
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
      if (elapsed >= item.ttl_) {
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
      if (ts.tv_sec <= item.ts_.tv_sec + item.ttl_) {
        block = false;
        break;
      }
    }
  } else {
    it = dnsMap_.find(ipFlowKey.reverse());
    if (it != dnsMap_.end()) {
      DnsItems& items = it.value();
      foreach (const DnsItem& item, items) {
        if (ts.tv_sec <= item.ts_.tv_sec + item.ttl_) {
          block = false;
          break;
        }
      }
    }
  }
  return block;
}


void GDnsFirewall::debugPacket(QString msg, GPacket* packet) {
  GPdus& pdus = packet->pdus_;

  QString proto = "unknown";
  GIp sip, dip;
  uint16_t sport = 0, dport = 0;

  GIpHdr* ipHdr = pdus.findFirst<GIpHdr>();
  if (ipHdr != nullptr) {
    proto = "ip";
    sip = ipHdr->sip();
    dip = ipHdr->dip();

    GTcpHdr* tcpHdr = pdus.findNext<GTcpHdr>();
    if (tcpHdr != nullptr) {
      proto = "tcp";
      sport = tcpHdr->sport();
      dport = tcpHdr->dport();
    }

    GUdpHdr* udpHdr = pdus.findNext<GUdpHdr>();
    if (udpHdr != nullptr) {
      proto = "udp";
      sport = udpHdr->sport();
      dport = udpHdr->dport();
    }
  }

  if (sport == 0 && dport == 0)
    qDebug() << QString("%1 %2 %3>%4").arg(msg, proto, qPrintable(sip), qPrintable(dip));
  else
    qDebug() << QString("%1 %2 %3:%4>:%5:%6").arg(msg, proto, qPrintable(sip), QString::number(sport), qPrintable(dip), QString::number(dport));
}

void GDnsFirewall::check(GPacket* packet) {
  GPdus& pdus = packet->pdus_;
  GIpHdr* ipHdr = pdus.findFirst<GIpHdr>();
  if (ipHdr == nullptr) {
    packet->control.block_ = false;
    emit notBlocked(packet);
    return;
  }

  bool decided = false;
  bool block = false;

  GTcpHdr* tcpHdr;
  GUdpHdr* udpHdr;
  if ((tcpHdr = pdus.findNext<GTcpHdr>()) != nullptr) {
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

  if (!decided && (udpHdr = pdus.findNext<GUdpHdr>()) != nullptr) {
    //
    // UDP
    //
    if (udpHdr->sport() == 53 || udpHdr->dport() == 53) {
      debugPacket("check dns", packet);
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

  packet->control.block_ = block;
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
  bool block = checkBlockFromDnsMap(ipFlowKey, packet->ts_);
  ipBlockMap_[ipFlowKey] = block;
  debugPacket("_ipFlowCreated " + QString(block ? "block" : "pass "), packet);
}

void GDnsFirewall::_ipFlowDeleted(GPacket* packet) {
  (void)packet;
  GFlow::IpFlowKey* key = ipFlowMgr_->key_;
  ipBlockMap_.remove(*key);
  debugPacket("_ipFlowDeleted", packet);
}

void GDnsFirewall::_tcpFlowCreated(GPacket* packet) {
  GFlow::TcpFlowKey* tcpFlowKey = tcpFlowMgr_->key_;
  GFlow::IpFlowKey ipFlowKey{tcpFlowKey->sip_, tcpFlowKey->dip_};
  bool block = checkBlockFromDnsMap(ipFlowKey, packet->ts_);
  tcpBlockMap_[*tcpFlowKey] = block;
  debugPacket("_tcpFlowCreated " + QString(block ? "block" : "pass "), packet);
}

void GDnsFirewall::_tcpFlowDeleted(GPacket* packet) {
  (void)packet;
  GFlow::TcpFlowKey* tcpFlowKey = tcpFlowMgr_->key_;
  tcpBlockMap_.remove(*tcpFlowKey);
  debugPacket("_tcpFlowDeleted", packet);
}

void GDnsFirewall::_udpFlowCreated(GPacket* packet) {
  GFlow::UdpFlowKey* udpFlowKey = udpFlowMgr_->key_;
  GFlow::IpFlowKey ipFlowKey{udpFlowKey->sip_, udpFlowKey->dip_};
  bool block = checkBlockFromDnsMap(ipFlowKey, packet->ts_);
  udpBlockMap_[*udpFlowKey] = block;
  debugPacket("_udpFlowCreated " + QString(block ? "block" : "pass "), packet);
}

void GDnsFirewall::_udpFlowDeleted(GPacket* packet) {
  (void)packet;
  GFlow::UdpFlowKey* udpFlowKey = udpFlowMgr_->key_;
  udpBlockMap_.remove(*udpFlowKey);
  debugPacket("_udpFlowDeleted", packet);
}

void GDnsFirewall::_dnsProcess(GPacket* packet, GDns* dns) {
  (void)packet;
  if (dns->answers_.count() == 0) return;

  GIpHdr* ipHdr = packet->pdus_.findFirst<GIpHdr>();
  Q_ASSERT(ipHdr != nullptr);
  GFlow::IpFlowKey ipFlowKey(ipHdr->dip(), uint32_t(0)); // ipHdr-dip() is dns query client ip

  foreach (GDns::ResourceRecord answer, dns->answers_) {
    if (answer.type_ != 0x0001) // Host Address
      continue;
    GIp ip = ntohl(*(uint32_t*)answer.data_.data());
    ipFlowKey.dip_ = ip; // dns response ip(server ip)
    qDebug() << QString("name=%1 ttl=%2 data=%3 %4").arg(answer.name_).arg(answer.ttl_).arg(qPrintable(ip), QString::number(ip, 16));

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
