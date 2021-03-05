#include "gdemonclient.h"
#include <QDebug>

// ----------------------------------------------------------------------------
// GDemonClient
// ----------------------------------------------------------------------------
GDemonClient::GDemonClient() {
}

GDemonClient::~GDemonClient() {
	disconnect();
}

bool GDemonClient::connect(std::string ip, uint16_t port) {
	sd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (sd_ == -1) {
		qWarning() << strerror(errno);
		return false;
	}

	struct in_addr ip_addr{0};
	int res = inet_pton(AF_INET, ip.data(), &ip_addr);
	switch (res) {
		case 1: break;
		case 0: qWarning() << strerror(errno); return false;
		case -1: qWarning() << strerror(errno); return false;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr = ip_addr;
	memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));

	res = ::connect(sd_, (struct sockaddr *)&addr, sizeof(addr));
	if (res == -1) {
		qWarning() << strerror(errno);
		::close(sd_);
		sd_ = 0;
		return -1;
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

GDemon::AllInterface GDemonClient::getAllInterface() {
	AllInterface allInterface;

	if (sd_ == 0) {
		qWarning() << "sd_ is 0";
		return allInterface;
	}

	char buffer[MaxBufferSize];
	GetAllInterfaceReq req;
	int32_t encLen = req.encode(buffer, MaxBufferSize);
	if (encLen == -1) {
		qWarning() << "req.encode return -1";
		return allInterface;
	}
	int sendLen = ::send(sd_, buffer, encLen, 0);
	if (sendLen == 0 || sendLen == -1) {
		qWarning() << "send return " << sendLen;
		return allInterface;
	}

	Header* header = GDemon::PHeader(buffer);
	if (!recvAll(sd_, header, sizeof(Header))) {
		qWarning() << "recvAll(header) return false";
		return allInterface;
	}

	if (!recvAll(sd_, buffer + sizeof(Header), header->len_)) {
		qWarning() << "recvAll(body) return false";
		return allInterface;
	}

	GetAllInterfaceRep rep;
	int32_t decLen = rep.decode(buffer, sizeof(Header) + header->len_);
	if (decLen == -1) {
		qWarning() << "rep.decode return -1";
		return allInterface;
	}

	return rep.allInterface_;
}
