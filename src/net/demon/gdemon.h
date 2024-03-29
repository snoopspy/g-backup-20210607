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
	typedef int64_t *pint64_t;
	typedef uint64_t *puint64_t;
	typedef bool *pbool;

	static const uint16_t DefaultPort = 8908;
	static const int MaxBufferSize = 32768;

	static bool recvAll(int sd, pvoid buffer, int32_t size);

	GDemon() {}
	virtual ~GDemon() {}

	enum Cmd: int32_t {
		CmdCmdExecute = 0,
		CmdCmdStart = 1,
		CmdCmdStop = 2,
		CmdCmdStartDetached = 3,
		CmdGetInterfaceList = 4,
		CmdGetRtm = 5,
		CmdPcapOpen = 6,
		CmdPcapClose = 7,
		CmdPcapRead = 8,
		CmdPcapWrite = 9
	};
	typedef Cmd *PCmd;

	//
	// header
	//
	struct Header {
		int32_t len_;
		Cmd cmd_;
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};
	typedef Header* PHeader;

	//
	// command
	//
	struct CmdExecuteReq : Header {
		std::string command_;
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};

	struct CmdExecuteRes : Header {
		bool result_{false};
		std::string error_;
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};

	struct CmdStartReq : Header {
		std::string command_;
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};

	struct CmdStartRes : Header {
		int64_t pid_{0};
		std::string error_;
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};

	struct CmdStopReq : Header {
		int64_t pid_{0};
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};

	struct CmdStopRes : Header {
		bool result_{false};
		std::string error_;
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};

	struct CmdStartDetachedReq : Header {
		std::string command_;
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};

	struct CmdStartDetachedRes : Header {
		bool result_{false};
		std::string error_;
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};

	//
	// network information
	//
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

	struct GetInterfaceListReq : Header {
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};

	struct GetInterfaceListRes : Header {
		InterfaceList interfaceList_;
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};

	struct GetRtmReq : Header {
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};

	struct GetRtmRes : Header {
		Rtm rtm_;
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};

	//
	// pcap
	//
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

	struct PcapOpenRes : Header {
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
		struct PktHdr {
			uint64_t tv_sec;
			uint64_t tv_usec;
			uint32_t caplen;
			uint32_t len;
		} pktHdr_;
		unsigned char* data_{nullptr};
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};

	struct PcapWrite : Header {
		int32_t size_;
		unsigned char* data_{nullptr};
		int32_t encode(pchar buffer, int32_t size);
		int32_t decode(pchar buffer, int32_t size);
	};
};
