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

#include <cassert>
#include <cstdint>
#include <cstring>
#include <list>
#include <string>
#include <pcap.h>
#include <unistd.h>

// ----------------------------------------------------------------------------
// GDemon
// ----------------------------------------------------------------------------
struct GDemon {
	typedef char *pchar;
	typedef void *pvoid;
	typedef int32_t *pint32_t;

	static const uint16_t DefaultPort = 8908;
	static const int MaxBufferSize = 8192;

	static bool readAll(int sd, pvoid buffer, int32_t size);

	GDemon() {}
	virtual ~GDemon() {}

	enum: int32_t {
		cmdGetInterfaceList = 0,
		cmdPcapOpen = 1,
		cmdPcapClose = 2
	};

	struct Interface {
		static const int MacSize = 6;
		int32_t index_{0};
		std::string name_;
		std::string desc_;
		uint8_t mac_[MacSize]{0,0,0,0,0,0};
		uint32_t ip_{0};
		uint32_t mask_{0};
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};

	struct InterfaceList : std::list<Interface> {
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};

	struct PcapOpen {
		std::string dev_;
		int32_t snaplen_;
		int32_t promisc_;
		int32_t timeout_;
		std::string errbuf_;
	};

	struct PcapClose {
		uint64_t pcap_;
	};

	struct PcapRead {
		uint64_t pcap_;
		struct pkthdr_;
		uint32_t size_;
		pchar* data;

		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};
};
