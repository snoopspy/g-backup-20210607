// ----------------------------------------------------------------------------
//
// G Library
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

#include "gdnsprocessor.h"
#include "net/flow/gflowmgr.h"
#include "net/flow/gipflowmgr.h"
#include "net/flow/gtcpflowmgr.h"
#include "net/flow/gudpflowmgr.h"

// ----------------------------------------------------------------------------
// GDnsFirewall
// ----------------------------------------------------------------------------
struct GDnsFirewall : GFlowMgr {
  Q_OBJECT
  Q_PROPERTY(GObjPtr ipFlowMgr READ getIpFlowMgr WRITE setIpFlowMgr)
  Q_PROPERTY(GObjPtr tcpFlowMgr READ getTcpFlowMgr WRITE setTcpFlowMgr)
  Q_PROPERTY(GObjPtr udpFlowMgr READ getUdpFlowMgr WRITE setUdpFlowMgr)
  Q_PROPERTY(GObjPtr dnsProcessor READ getDnsProcessor WRITE setDnsProcessor)

  GObjPtr getIpFlowMgr() { return ipFlowMgr_; }
  GObjPtr getTcpFlowMgr() { return tcpFlowMgr_; }
  GObjPtr getUdpFlowMgr() { return udpFlowMgr_; }
  GObjPtr getDnsProcessor() { return dnsProcessor_; }

  void setIpFlowMgr(GObjPtr value) { ipFlowMgr_ = dynamic_cast<GIpFlowMgr*>(value.data()); }
  void setTcpFlowMgr(GObjPtr value) { tcpFlowMgr_ = dynamic_cast<GTcpFlowMgr*>(value.data()); }
  void setUdpFlowMgr(GObjPtr value) { udpFlowMgr_ = dynamic_cast<GUdpFlowMgr*>(value.data()); }
  void setDnsProcessor(GObjPtr value) { dnsProcessor_ = dynamic_cast<GDnsProcessor*>(value.data()); }

public:
  GIpFlowMgr* ipFlowMgr_{nullptr};
  GTcpFlowMgr* tcpFlowMgr_{nullptr};
  GUdpFlowMgr* udpFlowMgr_{nullptr};
  GDnsProcessor* dnsProcessor_{nullptr};

public:
  // --------------------------------------------------------------------------
  // FlowItem
  // --------------------------------------------------------------------------
  struct FlowItem { // gilgil temp 2016.10.11
    int packets;
    int bytes;
  };
  // --------------------------------------------------------------------------

  // --------------------------------------------------------------------------
  struct DfItem {
    struct timeval ts_;
    long ttl_;
    QString name_;
  };

  struct DfItems : QList<DfItem> {
  };

  struct DfMap : QMap<GFlow::IpFlowKey, DfItems> {
  };

  // --------------------------------------------------------------------------

public:
  Q_INVOKABLE GDnsFirewall(QObject* parent = nullptr) : GFlowMgr(parent) {}
  ~GDnsFirewall() override { close(); }

protected:
  bool doOpen() override;
  bool doClose() override;

protected:
  size_t ipFlowOffset_{0};
  size_t tcpFlowOffset_{0};
  size_t udpFlowOffset_{0};

protected:
  DfMap dfMap_;
  void deleteOldFlowMaps(struct timeval ts);

public slots:
  void check(GPacket* packet);

  void _ipFlowCreated(const GFlow::IpFlowKey* key, GFlow::Value* value);
  void _ipFlowDeleted(const GFlow::IpFlowKey* key, GFlow::Value* value);

  void _tcpFlowCreated(const GFlow::TcpFlowKey* key, GFlow::Value* value);
  void _tcpFlowDeleted(const GFlow::TcpFlowKey* key, GFlow::Value* value);

  void _udpFlowCreated(const GFlow::UdpFlowKey* key, GFlow::Value* value);
  void _udpFlowDeleted(const GFlow::UdpFlowKey* key, GFlow::Value* value);

  void _dnsProcess(GPacket* packet, GDns* dns);

signals:
  void blocked(GPacket* packet);
  void notBlocked(GPacket* packet);
};
