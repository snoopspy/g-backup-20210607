#include "gbase.h"

#ifdef _DEBUG
const char* G::VERSION = "G Library version 0.1 Debug   Build( " __DATE__ " " __TIME__ " )";
#else // RELEASE
const char* G::VERSION = "G Library version 0.1 Release Build( " __DATE__ " " __TIME__ " )";
#endif // _DEBUG
const unsigned long G::TIMEOUT = 5000;
