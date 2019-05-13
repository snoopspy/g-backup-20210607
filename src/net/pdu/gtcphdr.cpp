#include "gtcphdr.h"

// ----------------------------------------------------------------------------
// GTcpHdr
// ----------------------------------------------------------------------------
//
// All tcpHdr field except tcpHdr.sum
// All data buffer(padding)
// ipHdr.ip_src, ipHdr.ip_dst, tcpHdrDataLen and IPPROTO_TCP
//
uint16_t GTcpHdr::calcChecksum(GIpHdr* ipHdr, GTcpHdr* tcpHdr) {
  uint32_t res = 0;
  int tcpHdrDataLen = ipHdr->hl() * 4 - sizeof(GIpHdr);

  // Add tcpHdr & data buffer as array of uint16_t
  uint16_t* p = reinterpret_cast<uint16_t*>(tcpHdr);
  for (int i = 0; i < tcpHdrDataLen / 2; i++) {
    res += htons(*p);
    p++;
  }

  // If length is odd, add last data(padding)
  if ((tcpHdrDataLen / 2) * 2 != tcpHdrDataLen)
    res += uint32_t(*(reinterpret_cast<uint8_t*>(p)) << 16);

  // Decrease checksum from sum
  res -= tcpHdr->sum();

  // Add src address
  uint32_t src = ipHdr->sip();
  res += ((src & 0xFFFF0000) >> 16) + (src & 0x0000FFFF);

  // Add dst address
  uint32_t dst = ipHdr->dip();
  res += ((dst & 0xFFFF0000) >> 16) + (dst & 0x0000FFFF);

  // Add extra information
  res += uint32_t(tcpHdrDataLen) + IPPROTO_TCP;

  // Recalculate sum
  while (res >> 16) {
    res = (res & 0xFFFF) + (res >> 16);
  }
  res = ~res;

  return uint16_t(res);
}

// ----------------------------------------------------------------------------
// GTEST
// ----------------------------------------------------------------------------
#ifdef GTEST
#include <gtest/gtest.h>

static u_char _ipHdr[] = "\x45\x00\x00\x28\x72\xd4\x40\x00\x40\x06\x55\xad\x0a\x01\x01\x02\xd3\xfb\x93\x50";
static u_char _tcpHdr[] = "\x80\x8a\x00\x50\xb1\x6f\x65\xf9\xa8\x93\x68\x2e\x50\x10\x00\xe5\x93\x9b\x00\x00";

TEST(GTcpHdr, hdrTest) {
  GTcpHdr* tcpHdr = reinterpret_cast<GTcpHdr*>(_tcpHdr);
  EXPECT_EQ(tcpHdr->dport(), 80);
  EXPECT_EQ(tcpHdr->off(), 4);
  EXPECT_EQ(tcpHdr->flags(), GTcpHdr::Ack);
}

TEST(GTcpHdr, checksumTest) {
  GIpHdr* ipHdr = reinterpret_cast<GIpHdr*>(_ipHdr);
  GTcpHdr* tcpHdr = reinterpret_cast<GTcpHdr*>(_tcpHdr);
  uint16_t realSum = tcpHdr->sum();
  uint16_t calcSum = GTcpHdr::calcChecksum(ipHdr, tcpHdr);
  EXPECT_EQ(realSum, calcSum);
}

#endif // GTEST
