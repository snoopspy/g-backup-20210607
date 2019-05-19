#include "gmac.h"

// ----------------------------------------------------------------------------
// GMac
// ----------------------------------------------------------------------------
GMac::GMac(const char* rhs) {
  int i;
  gbyte* p;
  gbyte ch1, ch2;

  p = reinterpret_cast<u_char*>(const_cast<char*>(rhs));
  for (i = 0 ; i < SIZE; i++)
  {
    ch1 = *p++;
    if (ch1 >= 'a')
      ch1 = ch1 - 'a' + 10;
    else
    if (ch1 >= 'A')
      ch1 = ch1 - 'A' + 10;
    else
      ch1 = ch1 - '0';
    ch2 = *p++;
    if (ch2 >= 'a')
      ch2 = ch2 - 'a' + 10;
    else if (ch2 >= 'A')
      ch2 = ch2 - 'A' + 10;
    else
      ch2 = ch2 - '0';
    mac_[i] = static_cast<gbyte>((ch1 << 4) + ch2);
    while (*p == '-' || *p == ':') p++;
  }
}

GMac::operator QString() const {
  gbyte ch1, ch2;
  int i, index;
  char buf[SIZE * 3]; // enough size

  index = 0;
  for (i = 0; i < SIZE; i++)
  {
    ch1 = mac_[i] & 0xF0;
    ch1 = ch1 >> 4;
    if (ch1 > 9)
      ch1 = ch1 + 'A' - 10;
    else
      ch1 = ch1 + '0';
    ch2 = mac_[i] & 0x0F;
    if (ch2 > 9)
      ch2 = ch2 + 'A' - 10;
    else
      ch2 = ch2 + '0';
    buf[index++] = char(ch1);
    buf[index++] = char(ch2);
    if (i == 2)
      buf[index++] = '-';
  }
  buf[index] = '\0';
  return (QString(reinterpret_cast<char*>(buf)));
}

GMac GMac::randomMac() {
  GMac res;
  for (int i = 0; i < SIZE; i++)
    res.mac_[i] = static_cast<gbyte>(rand() % 256);
  res.mac_[0] &= 0x7F;
  return res;
}

GMac& GMac::cleanMac() {
  static gbyte _value[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  static GMac res(_value);
  return res;
}

GMac& GMac::broadcastMac() {
  static gbyte _value[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
  static GMac res(_value);
  return res;
}

// ----------------------------------------------------------------------------
// GTEST
// ----------------------------------------------------------------------------
#ifdef GTEST
#include <gtest/gtest.h>

static u_char _temp[GMac::SIZE] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};

TEST(GMac, ctorTest) {
  GMac mac1; // ()

  GMac mac2{mac1}; // (const GMac& rhs)

  GMac mac3(_temp); // (const u_char* rhs)
  EXPECT_EQ(mac3, _temp);

  GMac mac4("001122-334455"); // (const char* rhs)
  EXPECT_EQ(mac4, _temp);

  GMac mac5(QString("001122-334455")); // (const QString& rhs)
  EXPECT_EQ(mac5, _temp);
}

TEST(GMac, castingTest) {
  GMac mac("001122-334455");

  const gbyte* uc = mac; // operator u_char*()
  gbyte temp[GMac::SIZE];
  for (int i = 0; i < GMac::SIZE; i++)
    temp[i] = *uc++;
  EXPECT_EQ(mac, temp);

  // ----- gilgil temp 2019.05.18 -----
  // Error occurrs on Windows
  /*
  QString s1 = static_cast<const char*>(mac); // operator const byte*()
  EXPECT_EQ(s1, "001122-334455");
  */
  // ----------------------------------

  QString s2 = QString(mac); // operator QString()
  EXPECT_EQ(s2, "001122-334455");
}

TEST(GMac, funcTest) {
  GMac mac;

  mac.clear();
  EXPECT_TRUE(mac.isClean());

  mac = "FF:FF:FF:FF:FF:FF";
  EXPECT_TRUE(mac.isBroadcast());

  mac = "01:00:5E:00:11:22";
  EXPECT_TRUE(mac.isMulticast());
}

#endif // GTEST
