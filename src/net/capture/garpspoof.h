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

#include "gpcapdevice.h"

// ----------------------------------------------------------------------------
// GArpSpoof
// ----------------------------------------------------------------------------
struct G_EXPORT GArpSpoof : GPcapDevice {
  Q_OBJECT

public:
  Q_INVOKABLE GArpSpoof(QObject* parent = nullptr);
  ~GArpSpoof() override;

protected:
  bool doOpen() override;
  bool doClose() override;
};
