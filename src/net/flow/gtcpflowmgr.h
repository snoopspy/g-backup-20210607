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
// GTcpFlowMgr
// ----------------------------------------------------------------------------
struct GTcpFlowMgr : GFlowMgr {
  Q_OBJECT
  Q_PROPERTY(long halfTimeout MEMBER halfTimeout_)
  Q_PROPERTY(long fullTimeout MEMBER fullTimeout_)
  Q_PROPERTY(long rstTimeout MEMBER rstTimeout_)
  Q_PROPERTY(long finTimeout MEMBER finTimeout_)

public:
  long halfTimeout_{60 * 1}; // 1 minutes
  long fullTimeout_{60 * 3}; // 3 minutes
  long rstTimeout_{10}; // 10 seconds
  long finTimeout_{20}; // 20 seconds

protected:
  // --------------------------------------------------------------------------
  // FlowMap
  // --------------------------------------------------------------------------
  struct FlowMap : QMap<GFlow::TcpFlowKey, GFlow::Value*> {
    void clear() {
      foreach (GFlow::Value* value, *this) {
        GFlow::Value::deallocate(value);
      }
      QMap<GFlow::TcpFlowKey, GFlow::Value*>::clear();
    }

    FlowMap::iterator erase(FlowMap::iterator it) {
      GFlow::Value* value = it.value();
      GFlow::Value::deallocate(value);
      return QMap<GFlow::TcpFlowKey, GFlow::Value*>::erase(it);
    }
  };
  // --------------------------------------------------------------------------

public:
  Q_INVOKABLE GTcpFlowMgr(QObject* parent = nullptr) : GFlowMgr(parent) {}
  ~GTcpFlowMgr() override { close(); }

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
  void deleteOldFlowMaps(struct timeval ts);

public:
  GFlow::TcpFlowKey* key_{nullptr};
  GFlow::Value* value_{nullptr};

public slots:
  void process(GPacket* packet);

signals:
  void processed(GPacket* packet);
  void _flowCreated(const GFlow::TcpFlowKey* key, GFlow::Value* value);
  void _flowDeleted(const GFlow::TcpFlowKey* key, GFlow::Value* value);
};
