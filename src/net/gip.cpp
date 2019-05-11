#include <arpa/inet.h>
#include "base/gerr.h"
#include "gip.h"

// ----------------------------------------------------------------------------
// GIp
// ----------------------------------------------------------------------------
GIp::GIp(const char* rhs) {
  int res = inet_pton(AF_INET, rhs, &ip_);
  switch (res) {
    case 0:
      qWarning() << "inet_pton return zero ip=" << rhs;
      break;
    case 1: // succeed
      ip_ = ntohl(ip_);
      break;
    default:
      qWarning() << "inet_pton return " << res << " " << GLastErr();
  }
}

GIp::operator QString() const {
  uint32_t ip = htonl(ip_);
  char s[INET_ADDRSTRLEN];
  const char* res = inet_ntop(AF_INET, &ip, s, INET_ADDRSTRLEN);
  if (res == nullptr) {
    qWarning() << "inet_ntop return null " << GLastErr();
    return QString();
  }
  return QString(s);
}

// ----------------------------------------------------------------------------
// GTEST
// ----------------------------------------------------------------------------
#ifdef GTEST
#include <gtest/gtest.h>

TEST(GIp, ctorTest) {
  GIp ip1; // ()

  GIp ip2{ip1}; // (const GIp&)

  GIp ip3{0x7F000001}; // (const uint32_t)
  EXPECT_EQ(ip3, 0x7F000001);

  GIp ip4{"127.0.0.1"}; // (const char*)
  EXPECT_EQ(ip4, 0x7F000001);

  GIp ip5{QString("127.0.0.1")}; // (const QString&)
  EXPECT_EQ(ip5, 0x7F000001);
}

TEST(GIp, castingTest) {
  GIp ip{"127.0.0.1"};

  uint32_t ui; ui = ip; // operator uint32_t() const
  EXPECT_EQ(ui, 0x7F000001);

  QString s1; s1 = static_cast<const char*>(ip); //operator const char*()
  EXPECT_EQ(s1, "127.0.0.1");

  QString s2; s2 = QString(ip); // operator QString()
  EXPECT_EQ(s2, "127.0.0.1");
}

TEST(GIp, assignmentTest) {
  GIp ip1{"127.0.0.1"};
  GIp ip2;

  ip2 = ip1; // GIp(const GIp&)
  EXPECT_EQ(ip2, 0x7F000001);

  ip2 = 0x7F000001; // GIp(const uint32_t)
  EXPECT_EQ(ip2, 0x7F000001);

  ip2 = "127.0.0.1"; // GIp(const char*)
  EXPECT_EQ(ip2, 0x7F000001);

  ip2 = QString("127.0.0.1"); // GIp(const QString&)
  EXPECT_EQ(ip2, 0x7F000001);
}

TEST(GIp, funcTest) {
  GIp ip;

  ip.clear();
  EXPECT_EQ(ip, 0);

  ip = "127.0.0.1";
  EXPECT_TRUE(ip.isLocalHost());

  ip = "255.255.255.255";
  EXPECT_TRUE(ip.isBroadcast());

  ip = "224.0.0.0";
  EXPECT_TRUE(ip.isMulticast());
}

#endif // GTEST

