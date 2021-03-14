#include "gdemon.h"
#include <sys/socket.h>
#include "gtrace.h"

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
	volatile pchar buf = buffer;

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
		GTRACE("GDemon::Interface::encode size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::Interface::decode(pchar buffer, int32_t size) {
	volatile pchar buf = buffer;

	// index_
	index_ = *pint32_t(buf); buf += sizeof(index_); size -= sizeof(index_);
	// GTRACE("index_ = %d", index_); // gilgil temp 2021.03.09

	// name_
	int32_t len = *pint32_t(buf); buf += sizeof(len); size -= sizeof(len);
	name_ = std::string(buf, len); buf += len; size -= len;
	// GTRACE("name_222=%s", name_.data()); // gilgil temp 2021.03.09

	// desc_
	len = *pint32_t(buf); buf += sizeof(len); size -= sizeof(len);
	desc_ = std::string(buf, len); buf += len; size -= len;
	// GTRACE("desc_=%s", desc_.data()); // gilgil temp 2021.03.09

	// mac_
	memcpy(mac_, buf, MacSize); buf += MacSize; size -= MacSize;
	// GTRACE("mac_222= %02x:%02x:%02x:%02x:%02x:%02x", mac_[0], mac_[1], mac_[2], mac_[3], mac_[4], mac_[5]); // gilgil temp 2021.03.09

	// ip_
	memcpy(&ip_, buf, sizeof(ip_)); buf += sizeof(ip_); size -= sizeof(ip_);

	// mask_
	memcpy(&mask_, buf, sizeof(mask_)); buf += sizeof(mask_); size -= sizeof(mask_);

	if (size < 0) {
		GTRACE("GDemon::Interface::decode size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::InterfaceList::encode(pchar buffer, int32_t size) {
	volatile pchar buf = buffer;

	// count
	int32_t cnt = this->size();
	*pint32_t(buf) = cnt; buf += sizeof(cnt); size -= sizeof(cnt);

	// InterfaceList
	for (Interface& intf: *this) {
		int32_t len = intf.encode(buf, size);
		buf += len; size -= len;
	}

	if (size < 0) {
		GTRACE("GDemon::InterfaceList::encode size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::InterfaceList::decode(pchar buffer, int32_t size) {
	volatile pchar buf = buffer;

	// count
	int32_t cnt = *pint32_t(buf); buf += sizeof(cnt); size -= sizeof(cnt);

	// InterfaceList
	for (int32_t i = 0; i < cnt; i++) {
		Interface interface;

		int32_t len = interface.decode(buf, size);
		//GTRACE("len=%d name=%s", len, interface.name_.data());
		push_back(interface);
		buf += len; size -= len;
		//GTRACE("len=%d name=%s size=%d", len, interface.name_.data(), size);
	}

	if (size < 0) {
		GTRACE("GDemon::InterfaceList::decode size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::Header::encode(pchar buffer, int32_t size) {
	volatile pchar buf = buffer;

	*pint32_t(buf) = len_; buf += sizeof(len_); size -= sizeof(len_);
	*PCmd(buf) = cmd_; buf += sizeof(cmd_); size -= sizeof(cmd_);

	if (size < 0) {
		GTRACE("GDemon::Header::encode size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::Header::decode(pchar buffer, int32_t size) {
	volatile pchar buf = buffer;

	len_ = *pint32_t(buf); buf += sizeof(len_); size -= sizeof(len_);
	cmd_ = *PCmd(buf); buf += sizeof(cmd_); size -= sizeof(cmd_);

	if (size < 0) {
		GTRACE("GDemon::Header::decode size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::GetInterfaceListReq::encode(pchar buffer, int32_t size) {
	volatile pchar buf = buffer;

	len_ = 0;
	cmd_ = cmdGetInterfaceList;
	int32_t encLen = Header::encode(buf, size);	buf += encLen; size -= encLen;

	if (size < 0) {
		GTRACE("GDemon::GetInterfaceListReq::encode size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::GetInterfaceListReq::decode(pchar buffer, int32_t size) {
	volatile pchar buf = buffer;

	int32_t decLen = Header::decode(buf, size); buf += decLen; // size -= decLen;
	if (len_ != 0) {
		GTRACE("len_ is not zero %d\n", len_);
		return -1;
	}
	if (cmd_ != cmdGetInterfaceList) {
		GTRACE("cmd_ is not cmdGetInterfaceList %d\n", cmd_);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::GetInterfaceListRep::encode(pchar buffer, int32_t size) {
	volatile pchar buf = buffer;

	buf += sizeof(Header); size -= sizeof(Header);

	int32_t encLen = interfaceList_.encode(buf, size); buf += encLen; size -= encLen;

	len_ = encLen;
	cmd_ = cmdGetInterfaceList;
	Header::encode(buffer, sizeof(Header)); // buf += encLen; size -= encLen; // gilgil temp 2021.03.05

	if (size < 0) {
		GTRACE("GDemon::GetInterfaceListRep::encode size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::GetInterfaceListRep::decode(pchar buffer, int32_t size) {
	volatile pchar buf = buffer;

	int32_t decLen = Header::decode(buf, size); buf += decLen; size -= decLen;
	if (cmd_ != cmdGetInterfaceList) {
		GTRACE("cmd_ is not cmdGetInterfaceList %d\n", cmd_);
		return -1;
	}

	decLen = interfaceList_.decode(buf, size); buf += decLen; size -= decLen;

	if (size < 0) {
		GTRACE("GDemon::GetInterfaceListRep::decode size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}
