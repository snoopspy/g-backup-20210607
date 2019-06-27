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

#include "gflowmgr.h"

// ----------------------------------------------------------------------------
// GUdpFlowMgr
// ----------------------------------------------------------------------------
struct G_EXPORT GUdpFlowMgr : GFlowMgr {
	Q_OBJECT
	Q_PROPERTY(long halfTimeout MEMBER halfTimeout_)
	Q_PROPERTY(long fullTimeout MEMBER fullTimeout_)

public:
	long halfTimeout_{60 * 1}; // 1 minutes
	long fullTimeout_{60 * 3}; // 3 minutes

public:
	// --------------------------------------------------------------------------
	// Managable
	// --------------------------------------------------------------------------
	struct Managable {
		virtual void udpFlowCreated(GFlow::UdpFlowKey* key, GFlow::Value* value) = 0;
		virtual void udpFlowDeleted(GFlow::UdpFlowKey* key, GFlow::Value* value) = 0;
	};
	typedef QSet<Managable*> Managables;
	Managables managables_;
	// --------------------------------------------------------------------------

protected:
	// --------------------------------------------------------------------------
	// FlowMap
	// --------------------------------------------------------------------------
	struct FlowMap : QMap<GFlow::UdpFlowKey, GFlow::Value*> {
		void clear() {
			for (GFlow::Value* value: *this) {
				GFlow::Value::deallocate(value);
			}
			QMap<GFlow::UdpFlowKey, GFlow::Value*>::clear();
		}

		FlowMap::iterator erase(FlowMap::iterator it) {
			GFlow::Value* value = it.value();
			GFlow::Value::deallocate(value);
			return QMap<GFlow::UdpFlowKey, GFlow::Value*>::erase(it);
		}
	};
	// --------------------------------------------------------------------------

public:
	Q_INVOKABLE GUdpFlowMgr(QObject* parent = nullptr) : GFlowMgr(parent) {}
	~GUdpFlowMgr() override { close(); }

protected:
	bool doOpen() override;
	bool doClose() override;

public:
	GFlow::RequestItems requestItems_;

protected:
	FlowMap flowMap_;
	void deleteOldFlowMaps(long now);

public:
	GFlow::UdpFlowKey key_;
	GFlow::Value* val_{nullptr};
	GFlow::UdpFlowKey rKey_;
	GFlow::Value* rVal_{nullptr};

public slots:
	void process(GPacket* packet);

signals:
	void processed(GPacket* packet);
};
