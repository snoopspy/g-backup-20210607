#include "gdemon.h"
#include <sys/socket.h>

// ----------------------------------------------------------------------------
// GDemon
// ----------------------------------------------------------------------------
bool GDemon::readAll(int sd, pvoid buf, int32_t size) {
	pchar _buf = pchar(buf);
	int32_t remain = size;
	while (true) {
		ssize_t once = ::recv(sd, _buf, remain, 0);
		if (once == 0 || once == -1)
			return false;
		_buf += once;
		remain -= once;
		if (remain == 0) break;
	}
	return true;
}

int32_t GDemon::Interface::encode(pchar buf, int32_t size) {
	int32_t res = 0;

	// index_
	*pint32_t(buf) = index_; res += sizeof(index_); buf += sizeof(index_);

	// name_
	int32_t len = name_.size();
	*pint32_t(buf) = int32_t(len); res += sizeof(len); buf += sizeof(len);
	memcpy(buf, name_.data(), len); res += len; buf += len;

	// desc_
	len = desc_.size();
	*pint32_t(buf) = int32_t(len); res += sizeof(len); buf += sizeof(len);
	memcpy(buf, desc_.data(), len); res += len; buf += len;

	// mac_
	memcpy(buf, mac_, MacSize); res += MacSize; buf += MacSize;

	// ip
	memcpy(buf, &ip_, sizeof(ip_)); res += sizeof(ip_); buf += sizeof(ip_);

	// mask_
	memcpy(buf, &mask_, sizeof(mask_)); res += sizeof(mask_); // buf += sizeof(mask_);

	if (res > size) {
		fprintf(stderr, "GDemon::Interface::encode size=%d res=%d\n", size, res);
		exit(-1);
	}
	return res;
}

int32_t GDemon::Interface::decode(pchar buf, int32_t size) {
	int32_t res = 0;

	// index_
	index_ = *pint32_t(buf); res += sizeof(index_); buf += sizeof(index_);

	// name_
	int32_t len = *pint32_t(buf); res += sizeof(len); buf += sizeof(len);
	name_ = std::string(pchar(buf), len); res += len; buf += len;

	// desc_
	len = *pint32_t(buf); res += sizeof(len); buf += sizeof(len);
	name_ = std::string(pchar(buf), len); res += len; // buf += len;

	if (res > size) {
		fprintf(stderr, "GDemon::Interface::decode size=%d res=%d\n", size, res);
		exit(-1);
	}
	return res;
}


int32_t GDemon::InterfaceList::encode(pchar buf, int32_t size) {
	int32_t res = 0;

	// count
	int32_t cnt = this->size();
	*pint32_t(buf) = cnt; res += sizeof(cnt); buf += sizeof(cnt);

	// InterfaceList
	for (Interface& intf: *this) {
		int32_t len = intf.encode(buf, size); res += len; buf += len;
	}

	if (res > size) {
		fprintf(stderr, "GDemon::InterfaceList::encode size=%d res=%d\n", size, res);
		exit(-1);
	}
	return res;
}

int32_t GDemon::InterfaceList::decode(pchar buf, int32_t size) {
	(void)buf; (void)size;
	return -1; // gilgil temp
}
