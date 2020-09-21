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

#include "gpdu.h"

// ----------------------------------------------------------------------------
// GArpHdr
// ----------------------------------------------------------------------------
#pragma pack(push, 1)
struct G_EXPORT GArpHdr final { // libnet_arp_hdr // gilgil temp 2019.05.26
	uint16_t hrd_;
	uint16_t pro_;
	uint8_t  hln_;
	uint8_t  pln_;
	uint16_t op_;
	GMac     smac_;
	GIp      sip_;
	GMac     tmac_;
	GIp      tip_;

	uint16_t hrd()  { return ntohs(hrd_); }
	uint16_t pro()  { return ntohs(pro_); }
	uint8_t  hln()  { return hln_;        }
	uint8_t  pln()  { return pln_;        }
	uint16_t op()   { return ntohs(op_);  }
	GMac     smac() { return smac_;       }
	GIp      sip()  { return ntohl(sip_); }
	GMac     tmac() { return tmac_;       }
	GIp      tip()  { return ntohl(tip_); }

	// HardwareType
	static const uint16_t NETROM   = 0;  // from KA9Q: NET/ROM pseudo
	static const uint16_t ETHER    = 1;  // Ethernet 10Mbps
	static const uint16_t EETHER   = 2;  // Experimental Ethernet
	static const uint16_t AX25     = 3;  // AX.25 Level 2
	static const uint16_t PRONET   = 4;  // PROnet token ring
	static const uint16_t CHAOS    = 5;  // Chaosnet
	static const uint16_t IEEE802  = 6;  // IEEE 802.2 Ethernet/TR/TB
	static const uint16_t ARCNET   = 7;  // ARCnet
	static const uint16_t APPLETLK = 8;  // APPLEtalk
	static const uint16_t LANSTAR  = 9;  // Lanstar
	static const uint16_t DLCI     = 15; // Frame Relay DLCI
	static const uint16_t ATM      = 19; // ATM
	static const uint16_t METRICOM = 23; // Metricom STRIP (new IANA id)
	static const uint16_t IPSEC    = 31;  // IPsec tunnel

	// Operation
	static const uint16_t Request    = 1; // req to resolve address
	static const uint16_t Reply      = 2; // resp to previous request
	static const uint16_t RevRequest = 3; // req protocol address given hardware
	static const uint16_t RevReply   = 4; // resp giving protocol address
	static const uint16_t InvRequest = 8; // req to identify peer
	static const uint16_t InvReply   = 9;  // resp identifying peer
};
typedef GArpHdr *PArpHdr;
#pragma pack(pop)
