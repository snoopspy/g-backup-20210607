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
// GFlowMgrTest
// ----------------------------------------------------------------------------
struct G_EXPORT GFlowMgrTest : GStateObj {
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
  // --------------------------------------------------------------------------

public:
  Q_INVOKABLE GFlowMgrTest(QObject* parent = nullptr) : GStateObj(parent) {}
  ~GFlowMgrTest() override { close(); }

protected:
  bool doOpen() override;
  bool doClose() override;

protected:
  size_t ipFlowOffset_{0};
  size_t tcpFlowOffset_{0};
  size_t udpFlowOffset_{0};

public slots:
  void test(GPacket* packet);

  void _ipFlowCreated(GPacket* packet);
  void _ipFlowDeleted(GPacket* packet);
  void _tcpFlowCreated(GPacket* packet);
  void _tcpFlowDeleted(GPacket* packet);
  void _udpFlowCreated(GPacket* packet);
  void _udpFlowDeleted(GPacket* packet);

signals:
  void tested(GPacket* packet);
};

