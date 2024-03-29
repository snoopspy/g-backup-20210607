// ----------------------------------------------------------------------------
//
// G Library
//
// http://gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

#include "base/gstateobj.h"
#include "net/flow/gtcpflowmgr.h"
#include "net/write/gwrite.h"

// ----------------------------------------------------------------------------
// GClientHelloSplit
// ----------------------------------------------------------------------------
struct G_EXPORT GClientHelloSplit : GStateObj, GTcpFlowMgr::Managable {
	Q_OBJECT
	Q_PROPERTY(GObjPtr tcpFlowMgr READ getTcpFlowMgr WRITE setTcpFlowMgr)
	Q_PROPERTY(GObjPtr write READ getWrite WRITE setWrite)

public:
	GObjPtr getTcpFlowMgr() { return tcpFlowMgr_; }
	void setTcpFlowMgr(GObjPtr value) { tcpFlowMgr_ = dynamic_cast<GTcpFlowMgr*>(value.data()); }
	GObjPtr getWrite() { return write_; }
	void setWrite(GObjPtr value) { write_ = dynamic_cast<GWrite*>(value.data()); }

public:
	GWrite* write_;
	GTcpFlowMgr* tcpFlowMgr_{nullptr};

	// --------------------------------------------------------------------------
	// FlowItem
	// --------------------------------------------------------------------------
	struct FlowItem {
		bool processed_{false};
	};
	typedef FlowItem *PFlowItem;
	// --------------------------------------------------------------------------

public:
	Q_INVOKABLE GClientHelloSplit(QObject* parent = nullptr) : GStateObj(parent) {}
	~GClientHelloSplit() override { close(); }

protected:
	bool doOpen() override;
	bool doClose() override;

protected:
	size_t tcpFlowOffset_{0};
	gbyte splittedTcpData_[GPacket::MaxBufSize];

public:
	// GTcpFlowMgr::Managable
	void tcpFlowCreated(GFlow::TcpFlowKey* key, GFlow::Value* value) override;
	void tcpFlowDeleted(GFlow::TcpFlowKey* key, GFlow::Value* value) override;

public slots:
	void split(GPacket* packet);
};
