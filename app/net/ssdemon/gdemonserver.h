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

	int accept_{0};

	bool start(uint16_t port = DefaultPort);
	void exec();
	void stop();
	void wait();

	struct SessionList;
	struct Session {
		int sd_{0};
		pcap_t* pcap_{nullptr};
		std::thread* t_{nullptr};

		static void _run(SessionList* sessions, Session* session);
		void run();

		void processGetInterfaceList(pchar buf, int32_t size);
		void processPcapOpen(pchar buf, int32_t size);
		void processPcapClose(pchar buf, int32_t size);
	};

	struct SessionList : std::list<Session*> {
	protected:
		std::mutex m_;
	public:
		void lock() { m_.lock(); }
		void unlock() { m_.unlock(); }
	} sessions_;
};
