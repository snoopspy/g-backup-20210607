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
  Q_PROPERTY(long ipFlowTimeout MEMBER ipFlowTimeout_)

public:
  long ipFlowTimeout_{60 * 5}; // 5 minutes

protected:
  // --------------------------------------------------------------------------
  // FlowMap
  // --------------------------------------------------------------------------
  struct FlowMap : QMap<GFlow::IpFlowKey, GFlow::Value*> {
    void clear() {
      for (FlowMap::iterator it = begin(); it != end(); it++) {
        GFlow::Value* value = it.value();
        free((void*)value);
      }
      QMap<GFlow::IpFlowKey, GFlow::Value*>::clear();
    }

    void erase(FlowMap::iterator it) {
      GFlow::Value* value = it.value();
      free((void*)value);
      QMap<GFlow::IpFlowKey, GFlow::Value*>::erase(it);
    }
  };
  // --------------------------------------------------------------------------

public:
  Q_INVOKABLE GIpFlowMgr(QObject* parent = nullptr) : GFlowMgr(parent) {}
  ~GIpFlowMgr() override { close(); }

protected:
  bool doOpen() override {
    // requestItems_.clear(); // gilgil temp 2016.10.10
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
  void deleteOldFlowMaps(struct timeval ts);

public:
  GFlow::IpFlowKey* key_;
  GFlow::Value* value_;

public slots:
  void process(GPacket* packet);

signals:
  void processed(GPacket* packet);
  void _flowCreated(const GFlow::IpFlowKey* key, GFlow::Value* value);
  void _flowDeleted(const GFlow::IpFlowKey* key, GFlow::Value* value);
};
