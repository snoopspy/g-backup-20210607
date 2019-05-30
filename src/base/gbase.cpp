#include "gbase.h"
#include <pcap.h>

#ifdef _DEBUG
const char* G::Version = "G Library version 0.2 Debug   Build( " __DATE__ " " __TIME__ " )";
#else // RELEASE
const char* G::Version = "G Library version 0.2 Release Build( " __DATE__ " " __TIME__ " )";
#endif // _DEBUG

const GDuration G::Timeout = 5000;

const char* G::pcap_lib_version() {
  return ::pcap_lib_version();
}

// ----------------------------------------------------------------------------
// GTEST
// ----------------------------------------------------------------------------
#ifdef GTEST
#include <gtest/gtest.h>

#include <iostream>
TEST(GBase, versionTest) {
  std::clog << G::Version << std::endl;
  std::clog << G::pcap_lib_version() << std::endl;
}

#endif // GTEST
