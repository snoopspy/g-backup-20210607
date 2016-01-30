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

#include <pcap.h>
#include <QList>
#include "net/gip.h"
#include "net/gmac.h"

// ----------------------------------------------------------------------------
// GNetInft
// ----------------------------------------------------------------------------
struct GNetIntfs;
struct GNetIntf {
  int index_{-1};
  QString name_;
  QString description_;
  pcap_if_t* dev_{nullptr};
  GMac mac_{GMac::cleanMac()};
  GIp ip_{(uint32_t)0};
  GIp mask_{(uint32_t)0};
  GIp gateway_{(uint32_t)0};

protected:
  GIp ip_and_mask_; // used for isSameLanIP

public:
  bool isSameLanIp(GIp ip) { return (ip_and_mask_) == (ip & mask_);   }
  GIp  getAdjIp(GIp ip)    { return isSameLanIp(ip) ? ip : gateway_;  }
  GIp  getStartIp()        { return (ip_ & mask_) + 1;                }
  GIp  getEndIp()          { return (ip_ | ~mask_);                   }

public:
  static GNetIntfs& all();
};

// ----------------------------------------------------------------------------
// GNetIntfs
// ----------------------------------------------------------------------------
struct GNetIntfs : QList<GNetIntf> {
  GNetIntfs();
  virtual ~GNetIntfs();
  pcap_if_t* allDevs_{nullptr};
  static GNetIntfs& instance();
};
