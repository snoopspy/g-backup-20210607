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

#include "gextract.h"
#include "net/flow/gtcpflowmgr.h"
#include <QRegularExpression>

// ----------------------------------------------------------------------------
// GHttpExtract
// ----------------------------------------------------------------------------
struct G_EXPORT GHttpExtract : GExtract, GTcpFlowMgr::Managable {
	Q_OBJECT
	Q_PROPERTY(QString folder MEMBER folder_)
	Q_PROPERTY(GObjPtr tcpFlowMgr READ getTcpFlowMgr WRITE setTcpFlowMgr)

public:
	GObjPtr getTcpFlowMgr() { return tcpFlowMgr_; }
	void setTcpFlowMgr(GObjPtr value) { tcpFlowMgr_ = dynamic_cast<GTcpFlowMgr*>(value.data()); }

public:
	QString folder_;
	GTcpFlowMgr* tcpFlowMgr_{nullptr};

public:
	// --------------------------------------------------------------------------
	// FlowItem
	// --------------------------------------------------------------------------
	struct FlowItem {
		GHttpExtract* httpExtract_;
		enum State {
			None,
			RequestHeaderCompleted,
			ResponseHeaderCompleted
		} state_;

		QString fileName_;
		size_t contentLength_;
		FILE* fp_;
		uint32_t firstClientServerSeq_;
		uint32_t firstServerClientSeq_;

		FlowItem(GHttpExtract* httpExtract);
		virtual ~FlowItem();

		void clear();
		void close();
	};
	typedef FlowItem *PFlowItem;
	// --------------------------------------------------------------------------

public:
	Q_INVOKABLE GHttpExtract(QObject* parent = nullptr) : GExtract(parent) {}
	~GHttpExtract() override { close(); }

protected:
	bool doOpen() override;
	bool doClose() override;

protected:
	size_t tcpFlowOffset_{0};

protected:
	QRegularExpression httpRequestFirstLine_{"GET ([^ ]+) HTTP/1.1\r\n"};
	QRegularExpression httpRequestHost_{"Host: ([^\r]+)\r\n"};
	QRegularExpression httpResponseFirstLine_{"HTTP/1.1 200"};
	QRegularExpression httpResponseContentLength_{"Content-Length: ([0-9]+)"};

public:
	// GTcpFlowMgr::Managable
	void tcpFlowCreated(GFlow::TcpFlowKey* key, GFlow::Value* value) override;
	void tcpFlowDeleted(GFlow::TcpFlowKey* key, GFlow::Value* value) override;

public slots:
	void write(GPacket* packet);

signals:
	void written(GPacket* packet);
	void finished(QString fileName);
};
