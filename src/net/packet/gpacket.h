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
#include <QObject>
#include "net/pdu/gpdu.h"
#include "net/pdu/gethhdr.h"
#include "net/pdu/garphdr.h"
#include "net/pdu/giphdr.h"
#include "net/pdu/gip6hdr.h"
#include "net/pdu/gtcphdr.h"
#include "net/pdu/gudphdr.h"
#include "net/pdu/gicmphdr.h"

// ----------------------------------------------------------------------------
// GPacket
// ----------------------------------------------------------------------------
struct G_EXPORT GPacket : QObject {
	Q_OBJECT
	Q_ENUMS(DataLinkType)

public:
	// --------------------------------------------------------------------------
	// Result
	// --------------------------------------------------------------------------
	typedef enum {
		Eof     = -2, // read
		Fail    = -1, // read write
		TimeOut = 0,  // read
		Ok      = 1,  // read write
	} Result;

	// --------------------------------------------------------------------------
	// DataLinkType
	// --------------------------------------------------------------------------
	typedef enum {
		Eth,   // DLT_EN10MB (1)
		Ip,    // DLT_RAW (228)
		Dot11, // DLT_IEEE802_11_RADIO (127)
		Null,  // DLT_NULL (0)
	} DataLinkType;
	static QString dataLinkTypeToString(DataLinkType dataLinkType);
	static int dataLinkTypeToInt(DataLinkType dataLinkType);
	static DataLinkType intToDataLinkType(int dataLink);
	// static GBuf getWriteBuf(GBuf buf, DataLinkType src, DataLinkType dst); // gilgil temp 2019.05.31

public:
	GPacket(QObject* parent = nullptr) : QObject(parent) { clear(); } // parent may be GCapture
	~GPacket() override { if (bufSelfAlloc_) free(buf_.data_); }

protected:
	DataLinkType dataLinkType_{Null};

public:
	//
	// info
	//
	DataLinkType dataLinkType() { return dataLinkType_; };

	//
	// sniffing
	//
	struct timeval ts_;
	GBuf buf_;
	bool bufSelfAlloc_{false};

	//
	// control
	//
	struct {
		bool block_{false};
		bool changed_{false};
	} ctrl;

	//
	// header
	//
	GEthHdr* ethHdr_{nullptr};
	GArpHdr* arpHdr_{nullptr};

	GIpHdr* ipHdr_{nullptr};
	GIp6Hdr* ip6Hdr_{nullptr};

	GTcpHdr* tcpHdr_{nullptr};
	GUdpHdr* udpHdr_{nullptr};
	GIcmpHdr* icmpHdr_{nullptr};

	GBuf tcpData_;
	GBuf udpData_;

	//
	// user
	//
	static constexpr int USER_DATA_SIZE = 256;
	gbyte userData_[USER_DATA_SIZE];

#ifdef _DEBUG
	//
	// debug
	//
	bool parsed_;
#endif // _DEBUG

public:
	void clear() {
		ts_.tv_sec = 0;
		ts_.tv_usec = 0;
		if (bufSelfAlloc_)
			free(buf_.data_);
		buf_.clear();
		ctrl.block_ = false;
		ctrl.changed_ = false;
		ethHdr_ = nullptr;
		arpHdr_ = nullptr;
		ipHdr_ = nullptr;
		ip6Hdr_ = nullptr;
		tcpHdr_ = nullptr;
		udpHdr_ = nullptr;
		icmpHdr_ = nullptr;
		tcpData_.clear();
		udpData_.clear();
#ifdef _DEBUG
		parsed_ = false;
#endif // _DEBUG
	}

	explicit operator QString() const;
	virtual void parse();

	virtual GPacket* clone(size_t extra  = 0);
	void doClone(GPacket* source, size_t extra);
};
typedef GPacket *PPacket;
