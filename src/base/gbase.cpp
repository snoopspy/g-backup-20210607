#include "gbase.h"

#ifdef _DEBUG
const char* G::Version = "G Library version 0.1 Debug   Build( " __DATE__ " " __TIME__ " )";
#else // RELEASE
const char* G::Version = "G Library version 0.1 Release Build( " __DATE__ " " __TIME__ " )";
#endif // _DEBUG

const unsigned long G::Timeout = 5000;

// ----------------------------------------------------------------------------
// GTEST
// ----------------------------------------------------------------------------
#ifdef GTEST
#include <gtest/gtest.h>

#include <iostream>
TEST(GBase, versionTest) {
  std::clog << G::Version << std::endl;
}

#endif // GTEST
