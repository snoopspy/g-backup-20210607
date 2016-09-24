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
  GCaptureThread(QObject* parent = nullptr) : GThread(parent) {}

protected:
  virtual void run();
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

  typedef enum {
    Eth,   // DLT_EN10MB
    Dot11, // DLT_IEEE802_11_RADIO
    Raw,   // DLT_RAW
    Null   // DLT_NULL
  } DataLinkType;

public:
  GCapture(QObject* parent = nullptr) : GStateObj(parent) {}
  ~GCapture() override;

protected:
  bool doOpen() override;
  bool doClose() override;

public:
  virtual GPacket::Result read(GPacket* packet);
  virtual GPacket::Result write(GPacket* packet);
  virtual GPacket::Result write(u_char* buf, size_t len);
  virtual GPacket::Result relay(GPacket* packet);

  virtual PathType pathType() { return OutOfPath; }
  virtual DataLinkType dataLinkType() { return Null; }

signals:
  void captured(GPacket* packet);

protected:
  GCaptureThread thread_{this};
  virtual void run();
};
