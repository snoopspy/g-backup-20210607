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
// GMac
// ----------------------------------------------------------------------------
struct G_EXPORT GMac final {
public:
	static constexpr int SIZE = 6;

protected:
	gbyte mac_[SIZE];

public:
	//
	// constructor
	//
	GMac() {}
	GMac(const GMac& rhs) { memcpy(this->mac_, rhs.mac_, SIZE); }
	GMac(const gbyte* rhs) { memcpy(this->mac_, rhs, SIZE); }
	GMac(const QString& rhs);

	// assign operator
	GMac& operator = (const GMac& rhs) { memcpy(this->mac_, rhs.mac_, SIZE); return *this; }

	//
	// casting operator
	//
	operator gbyte*() const { return const_cast<gbyte*>(mac_); } // default casting operator
	explicit operator QString() const;

	//
	// comparison operator
	//
	bool operator == (const GMac& rhs) const { return memcmp(mac_, rhs.mac_, SIZE) == 0; }
	bool operator != (const GMac& rhs) const { return memcmp(mac_, rhs.mac_, SIZE) != 0; }
	bool operator <  (const GMac& rhs) const { return memcmp(mac_, rhs.mac_, SIZE) <  0; }
	bool operator >  (const GMac& rhs) const { return memcmp(mac_, rhs.mac_, SIZE) >  0; }
	bool operator <= (const GMac& rhs) const { return memcmp(mac_, rhs.mac_, SIZE) <= 0; }
	bool operator >= (const GMac& rhs) const { return memcmp(mac_, rhs.mac_, SIZE) >= 0; }
	bool operator == (const pbyte rhs) const { return memcmp(mac_, rhs,      SIZE) == 0; }

public:
	void clear() {
		*this = cleanMac();
	}

	bool isClean() const {
		return *this == cleanMac();
	}

	bool isBroadcast() const { // FF:FF:FF:FF:FF:FF
		return *this == broadcastMac();
	}

	bool isMulticast() const { // 01:00:5E:0*
		return mac_[0] == 0x01 && mac_[1] == 0x00 && mac_[2] == 0x5E && (mac_[3] & 0x80) == 0x00;
	}

	static GMac randomMac();
	static GMac& cleanMac();
	static GMac& broadcastMac();
};
