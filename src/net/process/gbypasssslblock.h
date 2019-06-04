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
#include "net/flow/gtcpflowmgr.h"

// ----------------------------------------------------------------------------
// GBypassSslBlock
// ----------------------------------------------------------------------------
struct G_EXPORT GBypassSslBlock : GStateObj, GTcpFlowMgr::Managable {
  Q_OBJECT
  Q_PROPERTY(GObjPtr tcpFlowMgr READ getTcpFlowMgr WRITE setTcpFlowMgr)

public:
  GObjPtr getTcpFlowMgr() { return tcpFlowMgr_; }
  void setTcpFlowMgr(GObjPtr value) { tcpFlowMgr_ = dynamic_cast<GTcpFlowMgr*>(value.data()); }

public:
  GTcpFlowMgr* tcpFlowMgr_{nullptr};

  // --------------------------------------------------------------------------
  // FlowItem
  // --------------------------------------------------------------------------
  struct FlowItem {
    bool processed_;
  };
  typedef FlowItem *PFlowItem;
  // --------------------------------------------------------------------------

public:
  Q_INVOKABLE GBypassSslBlock(QObject* parent = nullptr) : GStateObj(parent) {}
  ~GBypassSslBlock() override { close(); }

protected:
  bool doOpen() override;
  bool doClose() override;

protected:
  size_t tcpFlowOffset_{0};

public:
  // GTcpFlowMgr::Managable
  void tcpFlowCreated(GFlow::TcpFlowKey* key, GFlow::Value* value) override;
  void tcpFlowDeleted(GFlow::TcpFlowKey* key, GFlow::Value* value) override;

public slots:
  void bypass(GPacket* packet);

signals:
  void bypassed(GPacket* packet);
};
