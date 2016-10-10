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
#include "net/flow/gudpflowmgr.h"

// ----------------------------------------------------------------------------
// GFlowMgrTest
// ----------------------------------------------------------------------------
struct GFlowMgrTest : GStateObj {
  Q_OBJECT
  Q_PROPERTY(GObjPtr ipFlowMgr READ getIpFlowMgr WRITE setIpFlowMgr)
  Q_PROPERTY(GObjPtr udpFlowMgr READ getUdpFlowMgr WRITE setUdpFlowMgr)

  GObjPtr getIpFlowMgr() { return ipFlowMgr_; }
  GObjPtr getUdpFlowMgr() { return udpFlowMgr_; }
  void setIpFlowMgr(GObjPtr value) { ipFlowMgr_ = dynamic_cast<GIpFlowMgr*>(value.data()); }
  void setUdpFlowMgr(GObjPtr value) { udpFlowMgr_ = dynamic_cast<GUdpFlowMgr*>(value.data()); }

public:
  GIpFlowMgr* ipFlowMgr_{nullptr};
  GUdpFlowMgr* udpFlowMgr_{nullptr};

public:
  // --------------------------------------------------------------------------
  // FlowItem
  // --------------------------------------------------------------------------
  struct FlowItem {
    int packets;
    int bytes;
  };
  // --------------------------------------------------------------------------

public:
  Q_INVOKABLE GFlowMgrTest(QObject* parent = nullptr) : GStateObj(parent) {}
  ~GFlowMgrTest() override { close(); }

protected:
  bool doOpen() override;
  bool doClose() override;

public:
  size_t ipFlowOffset_{0};
  size_t udpFlowOffset_{0};

public slots:
  void test(GPacket* packet);

  void _ipFlowCreated(const GFlow::IpFlowKey* key, GFlow::Value* value);
  void _ipFlowDeleted(const GFlow::IpFlowKey* key, GFlow::Value* value);
  void _udpFlowCreated(const GFlow::UdpFlowKey* key, GFlow::Value* value);
  void _udpFlowDeleted(const GFlow::UdpFlowKey* key, GFlow::Value* value);

signals:
  void tested(GPacket* packet);

};

