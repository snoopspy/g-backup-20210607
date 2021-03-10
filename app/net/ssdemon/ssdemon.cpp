#include <iostream>
#include "gdemonserver.h"
#include "gtrace.h"

using namespace std;

void usage() {
	cerr << "syntax: ssdemon [<port>]\n";
	cerr << "sample: ssdemon 8908\n";
}

struct Param {
	uint16_t port_{GDemon::DefaultPort};

	bool parse(int argc, char* argv[]) {
		if (argc >= 2)
			port_ = stoi(argv[1]);
		return true;
	}
} param;

int main(int argc, char* argv[]) {
	gtrace_default("127.0.0.1", 8908, true, nullptr);

	if (!param.parse(argc, argv)) {
		usage();
		return -1;
	}

	GTRACE("ssdemon started");
	GDemonServer server;
	if (!server.start(param.port_)) {
		return -1;
	}
	server.exec();
	server.stop();
	server.wait();
	GTRACE("ssdemon terminated");
}
