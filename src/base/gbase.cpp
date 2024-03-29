#include "gbase.h"
#include <pcap.h>


const char* G::version() {
#ifdef _DEBUG
	return "version "
#include "../../version.txt"
	" Debug Build(" __DATE__ " " __TIME__ ")";
#else // RELEASE
	return "version "
#include "../../version.txt"
	" Release Build(" __DATE__ " " __TIME__ ")";
#endif // _DEBUG
}

const char* G::pcap_lib_version() {
	return pchar(::pcap_lib_version());
}

// ----------------------------------------------------------------------------
// GTEST
// ----------------------------------------------------------------------------
#ifdef GTEST
#include <gtest/gtest.h>

#include <iostream>
TEST(GBase, versionTest) {
	std::cout << G::version() << std::endl;
	std::cout << G::pcap_lib_version() << std::endl;
}

#endif // GTEST
