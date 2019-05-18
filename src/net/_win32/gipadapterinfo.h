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

#include <winsock2.h>
#include <iphlpapi.h>

// ----------------------------------------------------------------------------
// GIpAdapterInfo
// ----------------------------------------------------------------------------
struct GIpAdapterInfo {
private: // singleton
  GIpAdapterInfo();
  virtual ~GIpAdapterInfo();

protected:
  PIP_ADAPTER_INFO pAdapterInfo_{nullptr};

public:
  PIP_ADAPTER_INFO findByAdapterName(char* value);
  static GIpAdapterInfo& instance();
};

