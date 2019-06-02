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
#include "net/write/gpcapdevicewriteethautomac.h"

// ----------------------------------------------------------------------------
// GBypassSslBlock
// ----------------------------------------------------------------------------
struct G_EXPORT GBypassSslBlock : GStateObj, GTcpFlowMgr::Managable {
  Q_OBJECT
  Q_PROPERTY(GObjPtr tcpFlowMgr READ getTcpFlowMgr WRITE setTcpFlowMgr)
  Q_PROPERTY(GObjPtr writer READ getWriter WRITE setWriter)

  GObjPtr getTcpFlowMgr() { return tcpFlowMgr_; }
  GObjPtr getWriter() { return writer_; }
  void setTcpFlowMgr(GObjPtr value) { tcpFlowMgr_ = dynamic_cast<GTcpFlowMgr*>(value.data()); }
  void setWriter(GObjPtr value) { writer_ = dynamic_cast<GPcapDeviceWriteEthAutoMac*>(value.data()); }

public:
  GTcpFlowMgr* tcpFlowMgr_{nullptr};
  GPcapDeviceWriteEthAutoMac* writer_{nullptr};

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
