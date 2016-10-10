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
  Q_PROPERTY(GObjPtr flowMgr READ getFlowMgr)

  GObjPtr getFlowMgr() { return flowMgr_; }

public:
  // --------------------------------------------------------------------------
  // Item
  // --------------------------------------------------------------------------
  struct Item {
    int packets;
    int bytes;
  };
  // --------------------------------------------------------------------------

public:
  Q_INVOKABLE GIpFlowMgrTest(QObject* parent = nullptr) : GStateObj(parent) {}
  ~GIpFlowMgrTest() override {}

protected:
  bool doOpen() override { return true; }
  bool doClose() override { return true; }

public:
  GIpFlowMgr* flowMgr_{nullptr};

public slots:
  void test(GPacket* packet);

signals:
  void tested(GPacket* packet);

};
