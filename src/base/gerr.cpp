#include "gerr.h"

// ----------------------------------------------------------------------------
// GErr
// ----------------------------------------------------------------------------
// std::ostream& operator << (std::ostream& os, GErr& err) { // gilgil temp 2015.10.28
QDebug operator << (QDebug os, GErr& err) {
  os
    << err.name() << ":"
    << (err.msg().isEmpty() ? "no msg" : err.msg())
    << " code=" << err.code();
  return os;
}

// std::ostream& operator << (std::ostream& os, GErr&& err) { // gilgil temp 2015.10.28
QDebug operator << (QDebug os, GErr&& err) {
  return operator << (os, err);
}

// std::ostream& operator << (QDebug &os, GErr* err) { // gilgil temp 2015.10.28
QDebug operator << (QDebug os, GErr* err) {
  if (err == nullptr) {
    os << "err is null";
    return os;
  }
  return operator << (os, *err);
}

// ----------------------------------------------------------------------------
// GTEST
// ----------------------------------------------------------------------------
#ifdef GTEST
#include <gtest/gtest.h>

TEST(GErr, lastErrTest) {
  int lastErrIdx = EHWPOISON;
  for (int n = 0; n <= lastErrIdx + 10; n++) {
    errno = n;
    GLastErr lastErr;
    qDebug() << lastErr;
    EXPECT_TRUE(lastErr.code() == n);
  }
}

TEST(GErr, stdErrTest) {
  GStdErr stdErr(GErr::NOT_SUPPORTED, "not supported");
  qDebug() << stdErr;
}

TEST(GErr, nullTest) {
  GLastErr* lastErr = nullptr;
  qDebug() << lastErr;
}

#endif // GTEST
