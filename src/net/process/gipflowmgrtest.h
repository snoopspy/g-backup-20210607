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

// ----------------------------------------------------------------------------
// GIpFlowMgrTest
// ----------------------------------------------------------------------------
struct GIpFlowMgrTest : GStateObj {
  Q_OBJECT
  Q_PROPERTY(GObjPtr flowMgr READ getFlowMgr WRITE setFlowMgr)

  GObjPtr getFlowMgr() { return flowMgr_; }
  void setFlowMgr(GObjPtr value) { flowMgr_ = dynamic_cast<GIpFlowMgr*>(value.data()); }

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
  Q_INVOKABLE GIpFlowMgrTest(QObject* parent = nullptr) : GStateObj(parent) {}
  ~GIpFlowMgrTest() override { close(); }

protected:
  bool doOpen() override;
  bool doClose() override;

public:
  GIpFlowMgr* flowMgr_{nullptr};
  size_t ipFlowOffset_{0};

public slots:
  void test(GPacket* packet);
  void _flowCreated(const GFlow::IpFlowKey* key, GFlow::Value* value);
  void _flowDeleted(const GFlow::IpFlowKey* key, GFlow::Value* value);

signals:
  void tested(GPacket* packet);

};

