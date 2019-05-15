// ----------------------------------------------------------------------------
//
// G Library
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

#include <QtCore>

#ifdef Q_OS_LINUX
  #include <arpa/inet.h>
  #include <netdb.h>
  #include <netinet/in.h>
  #include <sys/socket.h>
  #include <unistd.h>
#endif
#ifdef Q_OS_WIN
  #ifndef NOMINMAX
  #define NOMINMAX
  #endif // NOMINMAX
  #include <winsock2.h>
  #include <windows.h>
  #include <ws2tcpip.h>
#endif
