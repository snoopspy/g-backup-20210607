// ----------------------------------------------------------------------------
//
// G Library
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

#include "gnet.h"

// ----------------------------------------------------------------------------
// GIp
// ----------------------------------------------------------------------------
struct G_EXPORT GIp final {
public:
	static constexpr int SIZE = 4;

protected:
	uint32_t ip_;

public:
	//
	// constructor
	//
	GIp() {}
	GIp(const GIp& rhs) : ip_(rhs.ip_) {}
	GIp(const uint32_t rhs) : ip_(rhs) {}
	GIp(const QString& rhs);

	// assign operator
	GIp& operator = (const GIp& rhs) { ip_ = rhs.ip_; return *this; }

	//
	// casting operator
	//
	operator uint32_t() const { return ip_; } // default casting operator
	explicit operator QString() const;

public:
	void clear() { ip_ = 0; }

	bool isLocalHost() const { // 127.*.*.*
		uint8_t prefix = (ip_ & 0xFF000000) >> 24;
		return prefix == 0x7F;
	}

	bool isBroadcast() const { // 255.255.255.255
		return ip_ == 0xFFFFFFFF;
	}

	bool isMulticast() const { // 224.0.0.0 ~ 239.255.255.255
		uint8_t prefix = (ip_ & 0xFF000000) >> 24;
		return prefix >= 0xE0 && prefix < 0xF0;
	}
};
