#include "gdemon.h"
#include <sys/socket.h>

// ----------------------------------------------------------------------------
// GDemon
// ----------------------------------------------------------------------------
bool GDemon::recvAll(int sd, pvoid buffer, int32_t size) {
	pchar buf = pchar(buffer);
	int32_t remain = size;
	while (remain > 0) {
		ssize_t recvLen = ::recv(sd, buf, remain, 0);
		if (recvLen == 0 || recvLen == -1)
			return false;
		buf += recvLen;
		remain -= recvLen;
	}
	return true;
}

int32_t GDemon::Interface::encode(pchar buffer, int32_t size) {
	pchar buf = buffer;

	// index_
	*pint32_t(buf) = index_; buf += sizeof(index_); size -= sizeof(index_);

	// name_
	int32_t len = name_.size();
	*pint32_t(buf) = int32_t(len); buf += sizeof(len); size -= sizeof(len);
	memcpy(buf, name_.data(), len); buf += len; size -= len;

	// desc_
	len = desc_.size();
	*pint32_t(buf) = int32_t(len); buf += sizeof(len); size -= sizeof(len);
	memcpy(buf, desc_.data(), len); buf += len; size -= len;

	// mac_
	memcpy(buf, mac_, MacSize); buf += MacSize; size -= MacSize;

	// ip
	memcpy(buf, &ip_, sizeof(ip_)); buf += sizeof(ip_); size -= sizeof(ip_);

	// mask_
	memcpy(buf, &mask_, sizeof(mask_)); buf += sizeof(mask_); size -= sizeof(mask_);

	if (size < 0) {
		fprintf(stderr, "GDemon::Interface::encode size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::Interface::decode(pchar buffer, int32_t size) {
	pchar buf = buffer;

	// index_
	index_ = *pint32_t(buf); buf += sizeof(index_); size -= sizeof(index_);

	// name_
	int32_t len = *pint32_t(buf); buf += sizeof(len); size -= sizeof(len);
	name_ = std::string(pchar(buf), len); buf += len; size -= len;

	// desc_
	len = *pint32_t(buf); buf += sizeof(len); size -= sizeof(len);
	desc_ = std::string(pchar(buf), len); buf += len; size -= len;

	// mac_
	memcpy(mac_, buf, MacSize); buf += MacSize; size -= MacSize;

	// ip_
	memcpy(&ip_, buf, sizeof(ip_)); buf += sizeof(ip_); size -= sizeof(ip_);

	// mask_
	memcpy(&mask_, buf, sizeof(mask_)); buf += sizeof(mask_); size -= sizeof(mask_);

	if (size < 0) {
		fprintf(stderr, "GDemon::Interface::decode size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::AllInterface::encode(pchar buffer, int32_t size) {
	pchar buf = buffer;

	// count
	int32_t cnt = this->size();
	*pint32_t(buf) = cnt; buf += sizeof(cnt); size -= sizeof(cnt);

	// InterfaceList
	for (Interface& intf: *this) {
		int32_t len = intf.encode(buf, size);
		buf += len; size -= len;
	}

	if (size < 0) {
		fprintf(stderr, "GDemon::AllInterface::encode size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::AllInterface::decode(pchar buffer, int32_t size) {
	pchar buf = buffer;

	// count
	int32_t cnt = *pint32_t(buf); buf += sizeof(cnt); size -= sizeof(cnt);

	// InterfaceList
	for (int32_t i = 0; i < cnt; i++) {
		Interface interface;
		int32_t len = interface.decode(buf, size);
		push_back(interface);
		buf += len; size -= len;
	}

	if (size < 0) {
		fprintf(stderr, "GDemon::AllInterface::decode size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::Header::encode(pchar buffer, int32_t size) {
	pchar buf = buffer;

	*pint32_t(buf) = len_; buf += sizeof(len_); size -= sizeof(len_);
	*PCmd(buf) = cmd_; buf += sizeof(cmd_); size -= sizeof(cmd_);

	if (size < 0) {
		fprintf(stderr, "GDemon::Header::encode size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::Header::decode(pchar buffer, int32_t size) {
	pchar buf = buffer;

	len_ = *pint32_t(buf); buf += sizeof(len_); size -= sizeof(len_);
	cmd_ = *PCmd(buf); buf += sizeof(cmd_); size -= sizeof(cmd_);

	if (size < 0) {
		fprintf(stderr, "GDemon::Header::decode size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::GetAllInterfaceReq::encode(pchar buffer, int32_t size) {
	pchar buf = buffer;

	len_ = 0;
	cmd_ = cmdGetAllInterface;
	int32_t encLen = Header::encode(buf, size);	buf += encLen; size -= encLen;

	if (size < 0) {
		fprintf(stderr, "GDemon::GetAllInterfaceReq::encode size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::GetAllInterfaceReq::decode(pchar buffer, int32_t size) {
	pchar buf = buffer;

	int32_t decLen = Header::decode(buf, size); buf += decLen; // size -= decLen;
	if (len_ != 0) {
		fprintf(stderr, "len_ is not zero %d\n", len_);
		return -1;
	}
	if (cmd_ != cmdGetAllInterface) {
		fprintf(stderr, "cmd_ is not cmdGetAllInterface %d\n", cmd_);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::GetAllInterfaceRep::encode(pchar buffer, int32_t size) {
	pchar buf = buffer;

	buf += sizeof(Header); size -= sizeof(Header);

	int32_t encLen = allInterface_.encode(buf, size); buf += encLen; size -= encLen;

	len_ = encLen;
	cmd_ = cmdGetAllInterface;
	Header::encode(buffer, sizeof(Header)); // buf += encLen; size -= encLen; // gilgil temp 2021.03.05

	if (size < 0) {
		fprintf(stderr, "GDemon::GetAllInterfaceRep::encode size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::GetAllInterfaceRep::decode(pchar buffer, int32_t size) {
	pchar buf = buffer;

	int32_t decLen = Header::decode(buf, size); buf += decLen; size -= decLen;
	if (cmd_ != cmdGetAllInterface) {
		fprintf(stderr, "cmd_ is not cmdGetAllInterface %d\n", cmd_);
		return -1;
	}

	decLen = allInterface_.decode(buf, size); buf += decLen; size -= decLen;

	if (size < 0) {
		fprintf(stderr, "GDemon::GetAllInterfaceRep::decode size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}
