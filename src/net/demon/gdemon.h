// ----------------------------------------------------------------------------
//
// G Library
//
// http://gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

#include <cerrno>
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
	typedef unsigned char *puchar;
	typedef void *pvoid;
	typedef int32_t *pint32_t;
	typedef uint32_t *puint32_t;
	typedef bool *pbool;

	static const uint16_t DefaultPort = 8908;
	static const int MaxBufferSize = 8192;

	static bool recvAll(int sd, pvoid buffer, int32_t size);

	GDemon() {}
	virtual ~GDemon() {}

	enum Cmd: int32_t {
		CmdRunCommand = 0,
		CmdGetInterfaceList = 1,
		CmdGetRtm = 2,
		CmdPcapOpen = 3,
		CmdPcapClose = 4,
		CmdPcapRead = 5
	};
	typedef Cmd *PCmd;

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

	struct RtmEntry {
		uint32_t dst_{0};
		uint32_t mask_{0};
		uint32_t gateway_{0};
		int32_t metric_{0};
		std::string intfName_;
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};

	struct Rtm : std::list<RtmEntry> {
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};

	struct Header {
		int32_t len_;
		Cmd cmd_;
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};
	typedef Header* PHeader;

	struct GetInterfaceListReq : Header {
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};

	struct GetInterfaceListRep : Header {
		InterfaceList interfaceList_;
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};

	struct GetRtmReq : Header {
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};

	struct GetRtmRep : Header {
		Rtm rtm_;
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};

	struct PcapOpenReq : Header {
		std::string filter_;
		std::string intfName_;
		int32_t snaplen_;
		int32_t flags_;
		int32_t readTimeout_;
		int32_t waitTimeout_;
		bool captureThread_;
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};

	struct PcapOpenRep : Header {
		bool result_{false};
		int32_t dataLink_{0};
		std::string errBuf_{"no error"};
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};

	struct PcapCloseReq : Header {
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};

	struct PcapRead : Header {
		pcap_pkthdr pktHdr_;
		unsigned char* data_{nullptr};
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};
};
