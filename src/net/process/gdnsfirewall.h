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
  Q_PROPERTY(GObjPtr dnsProcessor READ getDnsProcessor WRITE setDnsProcessor)
  Q_PROPERTY(GObjPtr ipFlowMgr READ getIpFlowMgr WRITE setIpFlowMgr)
  Q_PROPERTY(GObjPtr tcpFlowMgr READ getTcpFlowMgr WRITE setTcpFlowMgr)
  Q_PROPERTY(GObjPtr udpFlowMgr READ getUdpFlowMgr WRITE setUdpFlowMgr)
  Q_PROPERTY(QStringList exceptionIpList MEMBER exceptionIpList_)
  Q_PROPERTY(long extraTtlTime MEMBER extraTtlTime_)

  GObjPtr getDnsProcessor() { return dnsProcessor_; }
  GObjPtr getIpFlowMgr() { return ipFlowMgr_; }
  GObjPtr getTcpFlowMgr() { return tcpFlowMgr_; }
  GObjPtr getUdpFlowMgr() { return udpFlowMgr_; }

  void setDnsProcessor(GObjPtr value) { dnsProcessor_ = dynamic_cast<GDnsProcessor*>(value.data()); }
  void setIpFlowMgr(GObjPtr value) { ipFlowMgr_ = dynamic_cast<GIpFlowMgr*>(value.data()); }
  void setTcpFlowMgr(GObjPtr value) { tcpFlowMgr_ = dynamic_cast<GTcpFlowMgr*>(value.data()); }
  void setUdpFlowMgr(GObjPtr value) { udpFlowMgr_ = dynamic_cast<GUdpFlowMgr*>(value.data()); }

public:
  GDnsProcessor* dnsProcessor_{nullptr};
  GIpFlowMgr* ipFlowMgr_{nullptr};
  GTcpFlowMgr* tcpFlowMgr_{nullptr};
  GUdpFlowMgr* udpFlowMgr_{nullptr};
  QStringList exceptionIpList_;
  QSet<GIp> exceptionIps_; // for exceptionIpList_
  long extraTtlTime_{10};

public:
  // --------------------------------------------------------------------------
  struct DnsItem {
    struct timeval ts_;
    long ttl_;
    QString name_;
  };

  struct DnsItems : QList<DnsItem> {};

  struct DnsMap : QMap<GFlow::IpFlowKey, DnsItems> {};

  struct IpBlockMap : QMap<GFlow::IpFlowKey, bool> {};
  struct TcpBlockMap : QMap<GFlow::TcpFlowKey, bool> {};
  struct UdpBlockMap : QMap<GFlow::UdpFlowKey, bool> {};
  // --------------------------------------------------------------------------

public:
  Q_INVOKABLE GDnsFirewall(QObject* parent = nullptr);
  ~GDnsFirewall() override { close(); }

protected:
  bool doOpen() override;
  bool doClose() override;

protected:
  DnsMap dnsMap_;
  IpBlockMap ipBlockMap_;
  TcpBlockMap tcpBlockMap_;
  UdpBlockMap udpBlockMap_;

  void deleteOldDnsMaps(GPacket* packet /* struct timeval ts */);
  bool checkBlockFromDnsMap(GFlow::IpFlowKey ipFlowKey, struct timeval ts);
  void debugPacket(QString msg, GPacket* packet);

public slots:
  void check(GPacket* packet);

  void _ipFlowCreated(GPacket* packet);
  void _ipFlowDeleted(GPacket* packet);

  void _tcpFlowCreated(GPacket* packet);
  void _tcpFlowDeleted(GPacket* packet);

  void _udpFlowCreated(GPacket* packet);
  void _udpFlowDeleted(GPacket* packet);

  void _dnsProcess(GPacket* packet, GDns* dns);

signals:
  void blocked(GPacket* packet);
  void notBlocked(GPacket* packet);
};
