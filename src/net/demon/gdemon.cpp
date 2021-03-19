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
		GTRACE("size is %d\n", size);
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
		GTRACE("size is %d\n", size);
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
		GTRACE("size is %d\n", size);
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
		GTRACE("size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::RtmEntry::encode(pchar buffer, int32_t size) {
	volatile pchar buf = buffer;

	// dst_
	*puint32_t(buf) = dst_; buf += sizeof(dst_); size -= sizeof(dst_);

	// mask_
	*puint32_t(buf) = mask_; buf += sizeof(mask_); size -= sizeof(mask_);

	// gateway_
	*puint32_t(buf) = gateway_; buf += sizeof(gateway_); size -= sizeof(gateway_);

	// metric_
	*puint32_t(buf) = metric_; buf += sizeof(metric_); size -= sizeof(metric_);

	// intfName_
	int32_t len = intfName_.size();
	*pint32_t(buf) = int32_t(len); buf += sizeof(len); size -= sizeof(len);
	memcpy(buf, intfName_.data(), len); buf += len; size -= len;

	if (size < 0) {
		GTRACE("size is %d\n", size);
		return -1;
	}
	return buf - buffer;

}

int32_t GDemon::RtmEntry::decode(pchar buffer, int32_t size) {
	volatile pchar buf = buffer;

	// dst_
	dst_ = *puint32_t(buf); buf += sizeof(dst_); size -= sizeof(dst_);

	// mask_
	mask_ = *puint32_t(buf); buf += sizeof(mask_); size -= sizeof(mask_);

	// dst_
	gateway_ = *puint32_t(buf); buf += sizeof(gateway_); size -= sizeof(gateway_);

	// dst_
	metric_ = *pint32_t(buf); buf += sizeof(metric_); size -= sizeof(metric_);

	// name_
	int32_t len = *pint32_t(buf); buf += sizeof(len); size -= sizeof(len);
	intfName_ = std::string(buf, len); buf += len; size -= len;

	if (size < 0) {
		GTRACE("GDemon::Interface::decode size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::Rtm::encode(pchar buffer, int32_t size) {
	volatile pchar buf = buffer;

	// count
	int32_t cnt = this->size();
	*pint32_t(buf) = cnt; buf += sizeof(cnt); size -= sizeof(cnt);

	// Rtm
	for (RtmEntry& rtmEntry: *this) {
		int32_t len = rtmEntry.encode(buf, size);
		buf += len; size -= len;
	}

	if (size < 0) {
		GTRACE("size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}


int32_t GDemon::Rtm::decode(pchar buffer, int32_t size) {
	volatile pchar buf = buffer;

	// count
	int32_t cnt = *pint32_t(buf); buf += sizeof(cnt); size -= sizeof(cnt);

	// Rtm
	for (int32_t i = 0; i < cnt; i++) {
		RtmEntry rtmEntry;

		int32_t len = rtmEntry.decode(buf, size);
		push_back(rtmEntry);
		buf += len; size -= len;
	}

	if (size < 0) {
		GTRACE("size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}


int32_t GDemon::Header::encode(pchar buffer, int32_t size) {
	volatile pchar buf = buffer;

	*pint32_t(buf) = len_; buf += sizeof(len_); size -= sizeof(len_);
	*PCmd(buf) = cmd_; buf += sizeof(cmd_); size -= sizeof(cmd_);

	if (size < 0) {
		GTRACE("size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::Header::decode(pchar buffer, int32_t size) {
	volatile pchar buf = buffer;

	len_ = *pint32_t(buf); buf += sizeof(len_); size -= sizeof(len_);
	cmd_ = *PCmd(buf); buf += sizeof(cmd_); size -= sizeof(cmd_);

	if (size < 0) {
		GTRACE("size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::GetInterfaceListReq::encode(pchar buffer, int32_t size) {
	volatile pchar buf = buffer;

	len_ = 0;
	cmd_ = cmdGetInterfaceList;
	int32_t encLen = Header::encode(buf, size); buf += encLen; size -= encLen;

	if (size < 0) {
		GTRACE("size is %d\n", size);
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
		GTRACE("size is %d\n", size);
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
		GTRACE("size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::GetRtmReq::encode(pchar buffer, int32_t size) {
	volatile pchar buf = buffer;

	len_ = 0;
	cmd_ = cmdGetRtm;
	int32_t encLen = Header::encode(buf, size);	buf += encLen; size -= encLen;

	if (size < 0) {
		GTRACE(" size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}


int32_t GDemon::GetRtmReq::decode(pchar buffer, int32_t size) {
	volatile pchar buf = buffer;

	int32_t decLen = Header::decode(buf, size); buf += decLen; // size -= decLen;
	if (len_ != 0) {
		GTRACE("len_ is not zero %d\n", len_);
		return -1;
	}
	if (cmd_ != cmdGetRtm) {
		GTRACE("cmd_ is not cmdGetRtm %d\n", cmd_);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::GetRtmRep::encode(pchar buffer, int32_t size) {
	volatile pchar buf = buffer;

	buf += sizeof(Header); size -= sizeof(Header);

	int32_t encLen = rtm_.encode(buf, size); buf += encLen; size -= encLen;

	len_ = encLen;
	cmd_ = cmdGetRtm;
	Header::encode(buffer, sizeof(Header)); // buf += encLen; size -= encLen; // gilgil temp 2021.03.05

	if (size < 0) {
		GTRACE(" size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}

int32_t GDemon::GetRtmRep::decode(pchar buffer, int32_t size) {
	volatile pchar buf = buffer;

	int32_t decLen = Header::decode(buf, size); buf += decLen; size -= decLen;
	if (cmd_ != cmdGetRtm) {
		GTRACE("cmd_ is not cmdGetRtm %d\n", cmd_);
		return -1;
	}

	decLen = rtm_.decode(buf, size); buf += decLen; size -= decLen;

	if (size < 0) {
		GTRACE(" size is %d\n", size);
		return -1;
	}
	return buf - buffer;
}
