#include "gdemonclient.h"
#include <QDebug>

// ----------------------------------------------------------------------------
// GDemonClient
// ----------------------------------------------------------------------------
GDemonClient::GDemonClient(std::string ip, uint16_t port) : ip_(ip), port_(port) {
	connect();
}

GDemonClient::~GDemonClient() {
	disconnect();
}

bool GDemonClient::connect() {
	if (sd_ != 0) {
		qDebug() << "already connected";
		return true;
	}

	sd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (sd_ == -1) {
		qWarning() << strerror(errno);
		sd_ = 0;
		return false;
	}

	struct in_addr ip_addr{0};
	int res = inet_pton(AF_INET, ip_.data(), &ip_addr);
	switch (res) {
		case 1: break;
		case 0: qWarning() << strerror(errno); return false;
		case -1: qWarning() << strerror(errno); return false;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port_);
	addr.sin_addr = ip_addr;
	memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));

	bool connected = false;
	for (int i = 0; i < 10 ; i++) { // 10 seconds
		res = ::connect(sd_, (struct sockaddr *)&addr, sizeof(addr));
		if (res != -1) {
			connected = true;
			break;
		}
		qWarning() << strerror(errno);
		sleep(1);
	}

	if (!connected) {
		::close(sd_);
		sd_ = 0;
		qCritical() << QString("can not connect to ssdemon %1 %2").arg(ip_.data()).arg(port_);
		return false;
	}
	return true;
}

bool GDemonClient::disconnect() {
	if (sd_ != 0) {
		::close(sd_);
		sd_ = 0;
	}

	return true;
}

GDemon::InterfaceList GDemonClient::getInterfaceList() {
	InterfaceList interfaceList;

	if (sd_ == 0) {
		qWarning() << "sd_ is 0";
		return interfaceList;
	}

	char buffer[MaxBufferSize];
	GetInterfaceListReq req;
	int32_t encLen = req.encode(buffer, MaxBufferSize);
	if (encLen == -1) {
		qWarning() << "req.encode return -1";
		return interfaceList;
	}
	int sendLen = ::send(sd_, buffer, encLen, 0);
	if (sendLen == 0 || sendLen == -1) {
		qWarning() << "send return " << sendLen;
		return interfaceList;
	}

	Header* header = GDemon::PHeader(buffer);
	if (!recvAll(sd_, header, sizeof(Header))) {
		qWarning() << "recvAll(header) return false";
		return interfaceList;
	}

	if (!recvAll(sd_, buffer + sizeof(Header), header->len_)) {
		qWarning() << "recvAll(body) return false";
		return interfaceList;
	}

	GetInterfaceListRep rep;
	int32_t decLen = rep.decode(buffer, sizeof(Header) + header->len_);
	if (decLen == -1) {
		qWarning() << "rep.decode return -1";
		return interfaceList;
	}

	return rep.interfaceList_;
}

GDemon::Rtm GDemonClient::getRtm() {
	Rtm rtm;

	if (sd_ == 0) {
		qWarning() << "sd_ is 0";
		return rtm;
	}

	char buffer[MaxBufferSize];
	GetRtmReq req;
	int32_t encLen = req.encode(buffer, MaxBufferSize);
	if (encLen == -1) {
		qWarning() << "req.encode return -1";
		return rtm;
	}
	int sendLen = ::send(sd_, buffer, encLen, 0);
	if (sendLen == 0 || sendLen == -1) {
		qWarning() << "send return " << sendLen;
		return rtm;
	}

	Header* header = GDemon::PHeader(buffer);
	if (!recvAll(sd_, header, sizeof(Header))) {
		qWarning() << "recvAll(header) return false";
		return rtm;
	}

	if (!recvAll(sd_, buffer + sizeof(Header), header->len_)) {
		qWarning() << "recvAll(body) return false";
		return rtm;
	}

	GetRtmRep rep;
	int32_t decLen = rep.decode(buffer, sizeof(Header) + header->len_);
	if (decLen == -1) {
		qWarning() << "rep.decode return -1";
		return rtm;
	}

	return rep.rtm_;
}

GDemonClient* GDemonClient::instance(std::string ip, uint16_t port) {
	static GDemonClientMap map;
	GDemonClientMapKey key(ip, port);
	GDemonClientMap::iterator it = map.find(key);
	if (it == map.end()) {
		map.insert({key, new GDemonClient(ip, port)});
		it = map.find(key);
	}
	return it->second;
}

// ----------------------------------------------------------------------------
// GDemonClientMap
// ----------------------------------------------------------------------------
GDemonClientMap::~GDemonClientMap() {
	for (GDemonClientMap::iterator it = begin(); it != end(); it++) {
		delete it->second;
	}
};

