#include "gerr.h"

// ----------------------------------------------------------------------------
// GErr
// ----------------------------------------------------------------------------
QDebug operator << (QDebug os, GErr& err) {
  os
    << err.name() << ":"
    << (err.msg().isEmpty() ? "no msg" : err.msg())
    << " code=" << err.code();
  return os;
}

QDebug operator << (QDebug os, GErr&& err) {
  return operator << (os, err);
}

QDebug operator << (QDebug os, GErr* err) {
  if (err == nullptr) {
    os << "err is null";
    return os;
  }
  return operator << (os, *err);
}

QDebug operator << (QDebug os, QSharedPointer<GErr> err) {
  return operator << (os, err.data());
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
