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
  // Q_PROPERTY(GObjP) // gilgil temp 2016.10.10

  // --------------------------------------------------------------------------
  // Item
  // --------------------------------------------------------------------------
  struct Item {
    int seq_{0};
  };

  // --------------------------------------------------------------------------

public:
  Q_INVOKABLE GIpFlowMgrTest(QObject* parent = nullptr) : GStateObj(parent) {}
  ~GIpFlowMgrTest() override {}

public:
  GIpFlowMgr* flowMgr_{nullptr};

public slots:
  void test(GPacket* packet);

signals:
  void tested(GPacket* packet);
};
