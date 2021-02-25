// ----------------------------------------------------------------------------
//
// G Library
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#include <cstdint>
#include <cstring>
#include <list>
#include <string>
#include <pcap.h>

// ----------------------------------------------------------------------------
// GDemon
// ----------------------------------------------------------------------------
struct GDemon {
	typedef char *pchar;
	typedef int32_t *pint32_t;

	static const uint16_t DefaultPort = 8908;
	static const int MaxBufferSize = 65536;

	enum: int32_t {
		getInterfaceList = 0,
		pcapOpen = 1,
		pcapClose = 2
	};

	struct Interface {
		int32_t index_;
		std::string name_;
		std::string desc_;
		int32_t encode(pchar buf, int32_t size);
		int32_t decode(pchar buf, int32_t size);
	};

	struct InterfaceList : std::list<Interface> {
		int32_t encode(pchar buf, int32_t size);
		int32_t decode(pchar buf, int32_t size);
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

		int32_t encode(pchar buf, int32_t size);
		int32_t decode(pchar buf, int32_t size);
	};
};

struct GDemonClient : GDemon {
	std::string error_;
	bool connect(std::string ip = "127.0.0.1", uint16_t port = DefaultPort);
	bool disconnect();

	InterfaceList getDeviceList();
};


#ifdef SSDEMON
struct GDemonServer: GDemon {
	std::string error_;
	bool start(std::string ip = "0.0.0.0", uint16_t port = DefaultPort);
	bool stop();
};
#endif // SSDEMON
