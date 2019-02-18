#include "gbase.h"

#ifdef _DEBUG
const char* G::Version = "G Library version 0.1 Debug   Build( " __DATE__ " " __TIME__ " )";
#else // RELEASE
const char* G::Version = "G Library version 0.1 Release Build( " __DATE__ " " __TIME__ " )";
#endif // _DEBUG
const unsigned long G::Timeout = 5000;
