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
#include "base/gthread.h"
#include "net/packet/gpacket.h"

// ----------------------------------------------------------------------------
// GCaptureThread
// ----------------------------------------------------------------------------
struct GCaptureThread : GThread {
  Q_OBJECT

public:
  GCaptureThread(QObject* parent = nullptr) : GThread(parent) {}

protected:
  void run() override;
};

// ----------------------------------------------------------------------------
// GCapture
// ----------------------------------------------------------------------------
struct GPacket;
struct GParser;
struct GCapture : GStateObj {
  Q_OBJECT
  Q_PROPERTY(bool enabled MEMBER enabled_)
  Q_PROPERTY(bool autoRead MEMBER autoRead_)
  Q_PROPERTY(bool autoParse MEMBER autoParse_)

public:
  bool enabled_{true};
  bool autoRead_{true};
  bool autoParse_{true};

public:
  friend class GCaptureThread;

  typedef enum {
    InPath,
    OutOfPath
  } PathType;

public:
  GCapture(QObject* parent = nullptr) : GStateObj(parent) {}
  ~GCapture() override;

protected:
  bool captureThreadOpen();
  bool captureThreadClose();

public:
  virtual GPacket::Result read(GPacket* packet);
  virtual GPacket::Result write(GPacket* packet);
  virtual GPacket::Result write(u_char* buf, size_t len);
  virtual GPacket::Result relay(GPacket* packet);

  virtual GPacket::DataLinkType dataLinkType() { return GPacket::Null; }
  virtual PathType pathType() { return OutOfPath; }

signals:
  void captured(GPacket* packet);

protected:
  GCaptureThread thread_{this};
  virtual void run();
};
