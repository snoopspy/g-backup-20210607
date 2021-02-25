#include "gdemonserver.h"
#include "gtrace.h"

// ----------------------------------------------------------------------------
// GDemonServer
// ----------------------------------------------------------------------------
GDemonServer::GDemonServer() {
}

GDemonServer::~GDemonServer() {
	stop();
	wait();
}

bool GDemonServer::start(uint16_t port) {
	accept_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if (accept_ == -1) {
		GTRACE("%s", strerror(errno));
		return false;
	}

	int optval = 1;
	int res = ::setsockopt(accept_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	if (res == -1) {
		GTRACE("%s", strerror(errno));
		return false;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	res = ::bind(accept_, (struct sockaddr *)&addr, sizeof(addr));
	if (res == -1) {
		GTRACE("%s", strerror(errno));
		return false;
	}

	res = listen(accept_, 5);
	if (res == -1) {
		GTRACE("%s", strerror(errno));
		return false;
	}

	return true;
}

void GDemonServer::exec() {
	while (true) {
		struct sockaddr_in cli_addr;
		socklen_t len = sizeof(cli_addr);
		int new_sd = accept(accept_, (struct sockaddr *)&cli_addr, &len);
		if (new_sd == -1) {
			GTRACE("%s", strerror(errno));
			break;
		}
		Session* session = new Session;
		session->sd_ = new_sd;
		session->t_ = new std::thread(Session::_run, &sessions_, session);
	}
}

void GDemonServer::stop() {
	if (accept_ != 0) {
		close(accept_);
		accept_ = 0;
	}

	sessions_.lock();
	for (Session* session: sessions_) {
		::close(session->sd_);
	}
	sessions_.unlock();
}

void GDemonServer::wait() {
	while(true) {
		sessions_.lock();
		bool exit = sessions_.size() == 0;
		sessions_.unlock();
		if (exit) break;
		sleep(1);
	}
}

void GDemonServer::Session::_run(SessionList* sessions, Session* session) {
	sessions->lock();
	sessions->push_back(session);
	sessions->unlock();

	session->run();

	sessions->lock();
	sessions->remove(session);
	sessions->unlock();
}

void GDemonServer::Session::run() {
	GTRACE("run beg");

	bool active = true;
	while (active) {
		int32_t len;
		if (!readAll(sd_, &len, sizeof(len)))
			break;

		char buffer[MaxBufferSize];
		if (!readAll(sd_, buffer, MaxBufferSize))
			break;

		pchar buf = buffer;
		int32_t cmd = *pint32_t(buf); buf += sizeof(cmd);
		switch (cmd) {
			case getInterfaceList:
				processGetInterfaceList(buf, len);
				break;
			case pcapOpen:
				processPcapOpen(buf, len);
				break;
			case pcapClose:
				processPcapClose(buf, len);
				break;
			default:
				GTRACE("invalid cmd %d", cmd);
				active = false;
				break;
		}
	}

	::close(sd_);
	if (pcap_ != nullptr)
		pcap_close(pcap_);

	GTRACE("run end");
}

void GDemonServer::Session::processGetInterfaceList(pchar buf, int32_t size) {
	GTRACE("processGetInterfaceList");
	(void)buf; (void)size;
}

void GDemonServer::Session::processPcapOpen(pchar buf, int32_t size) {
	GTRACE("processPcapOpen");
	(void)buf; (void)size;
}

void GDemonServer::Session::processPcapClose(pchar buf, int32_t size) {
	GTRACE("processPcapClose");
	(void)buf; (void)size;
}
