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

#include <list>
#include <mutex>
#include <thread>

#include "gdemon.h"

// ----------------------------------------------------------------------------
// GDemonServer
// ----------------------------------------------------------------------------
struct GDemonServer: GDemon {
	GDemonServer();
	~GDemonServer() override;

	bool start(uint16_t port = DefaultPort);
	void exec();
	void stop();
	void wait();

	int accept_{0};
	struct Session {
		Session();
		virtual ~Session();

		// Socket
		struct Socket {
			Session* owner_;
			int sd_{0};
			static void _run(GDemonServer* owner, int new_sd);
			void run();

			bool processGetInterfaceList(pchar buf, int32_t size);
			bool processGetRtm(pchar buf, int32_t size);
			bool processPcapOpen(pchar buf, int32_t size);
			bool processPcapClose(pchar buf, int32_t size);
		} socket;

		// pcap operation
		struct Pcap {
			Session* owner_;
			pcap_t* pcap_{nullptr};
			bool active_{false};
			std::thread* thread_{nullptr};

			PcapOpenRep open(PcapOpenReq req);
			void close();
			static void _run(Session* session, int waitTimeout);
			void run(int waitTimeout);
		} pcap;
	};

	struct SessionList : std::list<Session*> {
	protected:
		std::mutex m_;
	public:
		void lock() { m_.lock(); }
		void unlock() { m_.unlock(); }
	} sessions_;

protected:
	struct RtmFunc {
		static bool checkA(char* buf, RtmEntry* entry);
		static bool checkB(char* buf, RtmEntry* entry);
		static bool checkC(char* buf, RtmEntry* entry);
		static bool checkD(char* buf, RtmEntry* entry);
		static bool decodeCidr(std::string cidr, uint32_t* dst, uint32_t* mask);
		static uint32_t numberToMask(int number);
	};
};
