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

#include "gpcap.h"

// ----------------------------------------------------------------------------
// GPcapDevice
// ----------------------------------------------------------------------------
struct GPcapDevice : GPcap {
  Q_OBJECT
  Q_PROPERTY(QString devName     MEMBER devName_)
  Q_PROPERTY(int     snapLen     MEMBER snapLen_)
  Q_PROPERTY(int     flags       MEMBER flags_)
  Q_PROPERTY(int     readTimeout MEMBER readTimeout_)

public:
  QString devName_{""};
  int snapLen_{65536}; // 65536 bytes
  int flags_{1}; // PCAP_OPENFLAG_PROMISCUOUS
  int readTimeout_{1}; // 1 msec

public:
  Q_INVOKABLE GPcapDevice(QObject* parent = nullptr);
  ~GPcapDevice() override;

protected:
  bool doOpen() override;
  bool doClose() override;

#ifdef QT_GUI_LIB
  GPropItem* propCreateItem(GPropItemParam* param) override;
#endif // QT_GUI_LIB
};
