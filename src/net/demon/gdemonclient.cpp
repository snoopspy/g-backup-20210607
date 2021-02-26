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

GDemon::InterfaceList GDemonClient::getDeviceList() {
	InterfaceList interfaceList;

	if (sd_ == 0) {
		qWarning() << "sd_ is 0";
		return interfaceList;
	}

	char buffer[MaxBufferSize];
	pchar buf = buffer + sizeof(int32_t);
	int32_t cmd = cmdGetInterfaceList;
	int32_t writeLen;
	*pint32_t(buf) = cmd; writeLen = sizeof(cmd); // buf += sizeof(cmd);
	*pint32_t(buffer) = writeLen; writeLen += sizeof(writeLen);

	int res = ::send(sd_, buffer, writeLen, 0);
	if (res == 0 || res == -1) {
		qWarning() << "send return " << res;
		return interfaceList;
	}

	int32_t readLen;
	if (!readAll(sd_, &readLen, sizeof(readLen)))
		return interfaceList;

	if (!readAll(sd_, buffer, readLen))
		return interfaceList;

	interfaceList.decode(buffer, readLen);
	return interfaceList;
}
