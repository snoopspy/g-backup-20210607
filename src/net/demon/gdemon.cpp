#include "gdemon.h"
#include <sys/socket.h>

// ----------------------------------------------------------------------------
// GDemon
// ----------------------------------------------------------------------------
bool GDemon::readAll(int sd, pvoid buffer, int32_t size) {
	pchar buf = pchar(buffer);
	int32_t remain = size;
	while (true) {
		ssize_t once = ::recv(sd, buf, remain, 0);
		if (once == 0 || once == -1)
			return false;
		buf += once;
		remain -= once;
		if (remain == 0) break;
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
		fprintf(stderr, "GDemon::Interface::encode size=%d \n", size);
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
		fprintf(stderr, "GDemon::Interface::decode size=%d\n", size);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::InterfaceList::encode(pchar buffer, int32_t size) {
	pchar buf = buffer;

	// count
	int32_t cnt = this->size();
	*pint32_t(buf) = cnt; buf += sizeof(cnt); size -= sizeof(cnt);

	// InterfaceList
	for (Interface& intf: *this) {
		int32_t len = intf.encode(buf, size);
		if (len == -1) return -1;
		buf += len; size -= len;
	}

	if (size < 0) {
		fprintf(stderr, "GDemon::InterfaceList::encode size=%d\n", size);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::InterfaceList::decode(pchar buffer, int32_t size) {
	pchar buf = buffer;

	// count
	int32_t cnt = *pint32_t(buf); buf += sizeof(cnt); size -= sizeof(cnt);

	// InterfaceList
	for (int32_t i = 0; i < cnt; i++) {
		Interface interface;
		int32_t len = interface.decode(buf, size);
		if (len == -1) return -1;
		push_back(interface);
		buf += len; size -= len;
	}

	if (size < 0) {
		fprintf(stderr, "GDemon::InterfaceList::decode size=%d\n", size);
		return -1;
	}
	return buf - buffer;
}
