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
struct GIpFlowMgr : GFlowMgr {
  Q_OBJECT
  Q_PROPERTY(long halfTimeout MEMBER halfTimeout_)
  Q_PROPERTY(long fullTimeout MEMBER fullTimeout_)

public:
  long halfTimeout_{60 * 1}; // 1 minutes
  long fullTimeout_{60 * 3}; // 3 minutes

protected:
  // --------------------------------------------------------------------------
  // FlowMap
  // --------------------------------------------------------------------------
  struct FlowMap : QMap<GFlow::IpFlowKey, GFlow::Value*> {
    void clear() {
      foreach (GFlow::Value* value, *this) {
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
  bool doOpen() override {
    flowMap_.clear();
    return GFlowMgr::doOpen();
  }

  bool doClose() override {
    requestItems_.clear();
    flowMap_.clear();
    return GFlowMgr::doClose();
  }

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
  void _flowCreated(GPacket* packet);
  void _flowDeleted(GPacket* packet);
};
