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
// GIpFlowMgr
// ----------------------------------------------------------------------------
struct G_EXPORT GIpFlowMgr : GFlowMgr {
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
    virtual void ipFlowCreated(GFlow::IpFlowKey* key, GFlow::Value* value) = 0;
    virtual void ipFlowDeleted(GFlow::IpFlowKey* key, GFlow::Value* value) = 0;
  };
  typedef QSet<Managable*> Managables;
  Managables managables_;
  // --------------------------------------------------------------------------

protected:
  // --------------------------------------------------------------------------
  // FlowMap
  // --------------------------------------------------------------------------
  struct FlowMap : QMap<GFlow::IpFlowKey, GFlow::Value*> {
    void clear() {
      for (GFlow::Value* value: *this) {
        GFlow::Value::deallocate(value);
      }
      QMap<GFlow::IpFlowKey, GFlow::Value*>::clear();
    }

    FlowMap::iterator erase(FlowMap::iterator it) {
      GFlow::Value* value = it.value();
      GFlow::Value::deallocate(value);
      return QMap<GFlow::IpFlowKey, GFlow::Value*>::erase(it);
    }
  };
  // --------------------------------------------------------------------------

public:
  Q_INVOKABLE GIpFlowMgr(QObject* parent = nullptr) : GFlowMgr(parent) {}
  ~GIpFlowMgr() override { close(); }

protected:
  bool doOpen() override;
  bool doClose() override;

public:
  GFlow::RequestItems requestItems_;

protected:
  FlowMap flowMap_;
  void deleteOldFlowMaps(GPacket* packet /* struct timeval ts */);

public:
  GFlow::IpFlowKey* key_{nullptr};
  GFlow::Value* value_{nullptr};

public slots:
  void process(GPacket* packet);

signals:
  void processed(GPacket* packet);
};
