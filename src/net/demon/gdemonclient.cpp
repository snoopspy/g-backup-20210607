#include "gdemonclient.h"
#include <QDebug>

// ----------------------------------------------------------------------------
// GDemonClientMapKey
// ----------------------------------------------------------------------------
struct GDemonClientMapKey {
	GDemonClientMapKey(std::string ip, uint16_t port) : ip_(ip), port_(port) {}
	std::string ip_;
	uint16_t port_;
	bool operator<(const GDemonClientMapKey &r) const {
		if (ip_ < r.ip_) return true;
		if (ip_ > r.ip_) return false;
		if (port_ < r.port_) return true;
		if (port_ > r.port_) return false;
		return false;
	}
};

// ----------------------------------------------------------------------------
// GDemonClientMap
// ----------------------------------------------------------------------------
struct GDemonClientMap : std::map<GDemonClientMapKey, GDemonClient*> {
	friend struct GDemonClient;

protected: // singleton
	virtual ~GDemonClientMap() {
		for (GDemonClientMap::iterator it = begin(); it != end(); it++) {
			delete it->second;
		}
	}
};

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

	sd_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if (sd_ == -1) {
		error_ = strerror(errno);
		qWarning() << strerror(errno);
		sd_ = 0;
		return false;
	}

#ifdef __linux__
	in_addr_t ip_addr;
#endif // __linux__
#ifdef WIN32
	unsigned ip_addr;
#endif // WIN32
	ip_addr = inet_addr(ip_.data());

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port_);
	addr.sin_addr.s_addr = ip_addr;
	memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));

	bool connected = false;
	for (int i = 0; i < 10 ; i++) { // 10 seconds
		int res = ::connect(sd_, (struct sockaddr *)&addr, sizeof(addr));
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
		error_ = qPrintable(QString("can not connect to ssdemon %1 %2").arg(ip_.data()).arg(port_));
		qCritical() << error_.data();
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

GDemon::GetInterfaceListRep GDemonClient::getInterfaceList() {
	GetInterfaceListRep rep;

	if (sd_ == 0) {
		error_ = "not connected state";
		qWarning() << error_.data();
		return rep;
	}

	char buffer[MaxBufferSize];
	GetInterfaceListReq req;

	int32_t encLen = req.encode(buffer, MaxBufferSize);
	if (encLen == -1) {
		error_ = "req.encode return -1";
		qWarning() << error_.data();
		return rep;
	}

	int sendLen = ::send(sd_, buffer, encLen, 0);
	if (sendLen == 0 || sendLen == -1) {
		error_ = qPrintable(QString("send return %1").arg(sendLen));
		qWarning() << error_.data();
		return rep;
	}

	Header* header = GDemon::PHeader(buffer);
	if (!recvAll(sd_, header, sizeof(Header))) {
		error_ = "recvAll(header) return false";
		qWarning() << error_.data();
		return rep;
	}

	if (!recvAll(sd_, buffer + sizeof(Header), header->len_)) {
		error_ = "recvAll(body) return false";
		qWarning() << error_.data();
		return rep;
	}

	int32_t decLen = rep.decode(buffer, sizeof(Header) + header->len_);
	if (decLen == -1) {
		error_ = "rep.decode return -1";
		qWarning() << error_.data();
	}

	return rep;
}

GDemon::GetRtmRep GDemonClient::getRtm() {
	GetRtmRep rep;

	if (sd_ == 0) {
		error_ = "not connected state";
		qWarning() << error_.data();
		return rep;
	}

	char buffer[MaxBufferSize];
	GetRtmReq req;

	int32_t encLen = req.encode(buffer, MaxBufferSize);
	if (encLen == -1) {
		error_ = "req.encode return -1";
		qWarning() << error_.data();
		return rep;
	}

	int sendLen = ::send(sd_, buffer, encLen, 0);
	if (sendLen == 0 || sendLen == -1) {
		error_ = qPrintable(QString("send return %1").arg(sendLen));
		qWarning() << error_.data();
		return rep;
	}

	Header* header = GDemon::PHeader(buffer);
	if (!recvAll(sd_, header, sizeof(Header))) {
		error_ = "recvAll(header) return false";
		qWarning() << error_.data();
		return rep;
	}

	if (!recvAll(sd_, buffer + sizeof(Header), header->len_)) {
		error_ = "recvAll(body) return false";
		qWarning() << error_.data();
		return rep;
	}

	int32_t decLen = rep.decode(buffer, sizeof(Header) + header->len_);
	if (decLen == -1) {
		error_ = "rep.decode return -1";
		qWarning() << error_.data();
	}

	return rep;
}

GDemon::PcapOpenRep GDemonClient::pcapOpen(std::string filter, std::string intfName, int32_t snapLen, int32_t flags, int32_t readTimeout, int32_t waitTimeout, bool captureThread) {
	GDemon::PcapOpenRep rep;

	if (sd_ == 0) {
		error_ = "not connected state";
		qWarning() << error_.data();
		return rep;
	}

	char buffer[MaxBufferSize];
	GDemon::PcapOpenReq req;

	req.filter_ = filter;
	req.intfName_ = intfName;
	req.snaplen_ = snapLen;
	req.flags_ = flags;
	req.readTimeout_ = readTimeout;
	req.waitTimeout_ = waitTimeout;
	req.captureThread_ = captureThread;
	int32_t encLen = req.encode(buffer, MaxBufferSize);
	if (encLen == -1) {
		error_ = "req.encode return -1";
		return rep;
	}

	int sendLen = ::send(sd_, buffer, encLen, 0);
	if (sendLen == 0 || sendLen == -1) {
		error_ = qPrintable(QString("send return %1").arg(sendLen));
		qWarning() << error_.data();
		return rep;
	}

	Header* header = GDemon::PHeader(buffer);
	if (!recvAll(sd_, header, sizeof(Header))) {
		error_ = "recvAll(header) return false";
		qWarning() << error_.data();
		return rep;
	}

	if (!recvAll(sd_, buffer + sizeof(Header), header->len_)) {
		error_ = "recvAll(body) return false";
		qWarning() << error_.data();
		return rep;
	}

	int32_t decLen = rep.decode(buffer, sizeof(Header) + header->len_);
	if (decLen == -1) {
		qWarning() << "rep.decode return -1";
	}

	if (!rep.result_)
		error_ = rep.errBuf_;

	return rep;
}

void GDemonClient::pcapClose() {
	char buffer[MaxBufferSize];
	PcapCloseReq req;

	int32_t encLen = req.encode(buffer, MaxBufferSize);
	if (encLen == -1) {
		error_ = "req.encode return -1";
		qWarning() << error_.data();
		return;
	}

	int sendLen = ::send(sd_, buffer, encLen, 0);
	if (sendLen == 0 || sendLen == -1) {
		error_ = qPrintable(QString("send return %1").arg(sendLen));
		qWarning() << error_.data();
		return;
	}

	disconnect();
}

GDemon::PcapRead GDemonClient::pcapRead() {
	GDemon::PcapRead read;

	Header* header = GDemon::PHeader(readBuffer_);
	if (!recvAll(sd_, header, sizeof(Header))) {
		error_ = "recvAll(header) return false";
		qWarning() << error_.data();
		return read;
	}

	if (!recvAll(sd_, readBuffer_ + sizeof(Header), header->len_)) {
		error_ = "recvAll(body) return false";
		qWarning() << error_.data();
		return read;
	}

	int32_t decLen = read.decode(readBuffer_, sizeof(Header) + header->len_);
	if (decLen == -1) {
		error_ = "rep.decode return -1";
		qWarning() << error_.data();
		read.data_ = nullptr;
	}

	return read;
}

void GDemonClient::pcapWrite(PcapWrite write) {
	char buffer[MaxBufferSize];

	int32_t encLen = write.encode(buffer, MaxBufferSize);
	if (encLen == -1) {
		error_ = "req.encode return -1";
		qWarning() << error_.data();
		return;
	}

	int sendLen = ::send(sd_, buffer, encLen, 0);
	if (sendLen == 0 || sendLen == -1) {
		error_ = qPrintable(QString("send return %1").arg(sendLen));
		qWarning() << error_.data();
		return;
	}
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

