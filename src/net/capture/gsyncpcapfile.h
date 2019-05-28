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

#include "net/capture/gpcapfile.h"

// ----------------------------------------------------------------------------
// GSyncPcapFile
// ----------------------------------------------------------------------------
struct G_EXPORT GSyncPcapFile : GPcapFile {
  Q_OBJECT

public:
  Q_INVOKABLE GSyncPcapFile(QObject* parent = nullptr) : GPcapFile(parent) {
    autoRead_ = false;
  }
};
