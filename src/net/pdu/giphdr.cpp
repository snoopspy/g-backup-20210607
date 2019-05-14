#include "giphdr.h"

// ----------------------------------------------------------------------------
// GIpHdr
// ----------------------------------------------------------------------------
//
// All ipHdr field except ipHdr.sum
//
uint16_t GIpHdr::calcChecksum(GIpHdr* ipHdr) {
    uint32_t res = 0;
    uint16_t *p;

    // Add ipHdr buffer as array of uint16_t
    p = reinterpret_cast<uint16_t*>(ipHdr);
    for (int i = 0; i < int(sizeof(GIpHdr)) / 2; i++) {
      res += ntohs(*p);
      p++;
    }

    // Do not consider padding because ip header length is always multilpe of 2.

    // Decrease checksum from sum
    res -= ipHdr->sum();

    // Recalculate sum
    while (res >> 16) {
      res = (res & 0xFFFF) + (res >> 16);
    }
    res = ~res;

    return uint16_t(res);
}

uint16_t GIpHdr::recalcChecksum(uint16_t oldChecksum, uint16_t oldValue, uint16_t newValue) {
  uint32_t res = oldValue + (~newValue & 0xFFFF);
  res += oldChecksum;
  res = (res & 0xFFFF) + (res >> 16);
  return uint16_t(res + (res >> 16));
}

uint16_t GIpHdr::recalcChecksum(uint16_t oldChecksum, uint32_t oldValue, uint32_t newValue) {
  uint16_t oldValue16;
  uint16_t newValue16;
  uint16_t res;

  oldValue16 = (oldValue & 0xFFFF0000) >> 16;
  newValue16 = (newValue & 0xFFFF0000) >> 16;
  res = recalcChecksum(oldChecksum, oldValue16, newValue16);

  oldValue16 = oldValue & 0x0000FFFF;
  newValue16 = newValue & 0x0000FFFF;
  res = recalcChecksum(res, oldValue16, newValue16);

  return res;
}

// ----------------------------------------------------------------------------
// GTEST
// ----------------------------------------------------------------------------
#ifdef GTEST
#include <gtest/gtest.h>

static u_char _ipHdr[] = "\x45\x00\x00\x54\x65\x16\x40\x00\x40\x01\xc6\x8a\x0a\x01\x01\x02\x01\x02\x03\x04";

TEST(GIpHdr, hdrTest) {
  GIpHdr* ipHdr = reinterpret_cast<GIpHdr*>(_ipHdr);
  EXPECT_EQ(ipHdr->v(), 4);
  EXPECT_EQ(ipHdr->hl(), 5);
  EXPECT_EQ(ipHdr->sip(), GIp("10.1.1.2"));
  EXPECT_EQ(ipHdr->dip(), GIp("1.2.3.4"));
}

TEST(GIpHdr, checksumTest) {
  {
    GIpHdr* ipHdr = reinterpret_cast<GIpHdr*>(_ipHdr);
    uint16_t realSum = ipHdr->sum();
    uint16_t calcSum = GIpHdr::calcChecksum(ipHdr);
    EXPECT_EQ(realSum, calcSum);
  }

  {
    u_char temp[sizeof(_ipHdr)];
    memcpy(temp, _ipHdr, sizeof(_ipHdr));
    GIpHdr* ipHdr = reinterpret_cast<GIpHdr*>(temp);

    uint16_t oldChecksum = ipHdr->sum();
    uint16_t oldValue = uint16_t(ipHdr->ttl() << 8) + ipHdr->p();
    ipHdr->ttl_--;
    uint16_t newValue = uint16_t(ipHdr->ttl() << 8) + ipHdr->p();
    uint16_t newChecksum = GIpHdr::calcChecksum(ipHdr);
    uint16_t recalcChecksum = GIpHdr::recalcChecksum(oldChecksum, oldValue, newValue);
    EXPECT_EQ(newChecksum, recalcChecksum);
  }

  {
    u_char temp[sizeof(_ipHdr)];
    memcpy(temp, _ipHdr, sizeof(_ipHdr));
    GIpHdr* ipHdr = reinterpret_cast<GIpHdr*>(temp);

    uint16_t oldChecksum = ipHdr->sum();
    uint32_t oldValue = ipHdr->dip();
    ipHdr->dip_ = GIp("1.2.3.3");
    uint32_t newValue = ipHdr->dip();
    uint16_t newChecksum = GIpHdr::calcChecksum(ipHdr);
    uint16_t recalcChecksum = GIpHdr::recalcChecksum(oldChecksum, oldValue, newValue);
    EXPECT_EQ(newChecksum, recalcChecksum);
  }
}

#endif // GTEST
