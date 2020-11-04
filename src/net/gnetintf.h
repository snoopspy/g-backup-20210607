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
#include "gnet.h"
#include "gip.h"
#include "gmac.h"

// ----------------------------------------------------------------------------
// GNetIntf
// ----------------------------------------------------------------------------
struct GNetIntfs;
struct G_EXPORT GNetIntf {
	friend struct GNetIntfs;

public:
	int index() const { return index_; }
	QString name() const { return name_; }
	QString desc() const { return desc_; }
	pcap_if_t* dev() const { return dev_; }
	GMac mac() const { return mac_; }
	GIp ip() const { return ip_; }
	GIp mask() const { return mask_; }
	GIp gateway() const;

protected:
	int index_{-1};
	QString name_;
	QString desc_;
	pcap_if_t* dev_{nullptr};
	GMac mac_{GMac::nullMac()};
	GIp ip_{0};
	GIp mask_{0};
	GIp gateway_{0};

protected:
	GIp ip_and_mask_{0}; // used for isSameLanIP

public:
	GNetIntf() {}

public:
	bool isSameLanIp(GIp ip) { return (ip_and_mask_) == (ip & mask_);   }
	GIp  getAdjIp(GIp ip)    { return isSameLanIp(ip) ? ip : gateway_;  }
	GIp  getStartIp()        { return (ip_ & mask_) + 1;                }
	GIp  getEndIp()          { return (ip_ | ~mask_);                   }

public:
	static GNetIntfs& all();

	bool operator==(const GNetIntf& r) const;
};
uint qHash(GNetIntf q);

// ----------------------------------------------------------------------------
// GNetIntfs
// ----------------------------------------------------------------------------
struct G_EXPORT GNetIntfs : QList<GNetIntf> {
	friend struct GNetIntf;

private: // singleton
	GNetIntfs();
	virtual ~GNetIntfs();

public:
	GNetIntf* findByName(QString name);
	GNetIntf* findByIp(GIp ip);
	pcap_if_t* allDevs_{nullptr};
	static GNetIntfs& instance();

public:
	bool initialized_{false};
	void init();
};
