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

#include "base/gstateobj.h"
#include "net/flow/gipflowmgr.h"
#include "net/flow/gtcpflowmgr.h"
#include "net/flow/gudpflowmgr.h"

// ----------------------------------------------------------------------------
// GFlowMgrDebugger
// ----------------------------------------------------------------------------
struct G_EXPORT GFlowMgrDebugger : GStateObj, GIpFlowMgr::Managable, GTcpFlowMgr::Managable, GUdpFlowMgr::Managable {
  Q_OBJECT
  Q_PROPERTY(GObjPtr ipFlowMgr READ getIpFlowMgr WRITE setIpFlowMgr)
  Q_PROPERTY(GObjPtr tcpFlowMgr READ getTcpFlowMgr WRITE setTcpFlowMgr)
  Q_PROPERTY(GObjPtr udpFlowMgr READ getUdpFlowMgr WRITE setUdpFlowMgr)

  GObjPtr getIpFlowMgr() { return ipFlowMgr_; }
  GObjPtr getTcpFlowMgr() { return tcpFlowMgr_; }
  GObjPtr getUdpFlowMgr() { return udpFlowMgr_; }

  void setIpFlowMgr(GObjPtr value) { ipFlowMgr_ = dynamic_cast<GIpFlowMgr*>(value.data()); }
  void setTcpFlowMgr(GObjPtr value) { tcpFlowMgr_ = dynamic_cast<GTcpFlowMgr*>(value.data()); }
  void setUdpFlowMgr(GObjPtr value) { udpFlowMgr_ = dynamic_cast<GUdpFlowMgr*>(value.data()); }

public:
  GIpFlowMgr* ipFlowMgr_{nullptr};
  GTcpFlowMgr* tcpFlowMgr_{nullptr};
  GUdpFlowMgr* udpFlowMgr_{nullptr};

public:
  // --------------------------------------------------------------------------
  // FlowItem
  // --------------------------------------------------------------------------
  struct FlowItem {
    size_t packets;
    size_t bytes;
  };
  typedef FlowItem *PFlowItem;
  // --------------------------------------------------------------------------

public:
  Q_INVOKABLE GFlowMgrDebugger(QObject* parent = nullptr) : GStateObj(parent) {}
  ~GFlowMgrDebugger() override { close(); }

protected:
  bool doOpen() override;
  bool doClose() override;

protected:
  size_t ipFlowOffset_{0};
  size_t tcpFlowOffset_{0};
  size_t udpFlowOffset_{0};

public:
  // GIpFlowMgr::Managable
  void ipFlowCreated(GFlow::IpFlowKey* key, GFlow::Value* value) override;
  void ipFlowDeleted(GFlow::IpFlowKey* key, GFlow::Value* value) override;

  // GTcpFlowMgr::Managable
  void tcpFlowCreated(GFlow::TcpFlowKey* key, GFlow::Value* value) override;
  void tcpFlowDeleted(GFlow::TcpFlowKey* key, GFlow::Value* value) override;

  // GUdpFlowMgr::Managable
  void udpFlowCreated(GFlow::UdpFlowKey* key, GFlow::Value* value) override;
  void udpFlowDeleted(GFlow::UdpFlowKey* key, GFlow::Value* value) override;

public slots:
  void debug(GPacket* packet);

signals:
  void debugged(GPacket* packet);
};
