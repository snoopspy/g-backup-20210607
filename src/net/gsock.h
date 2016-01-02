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

#include <sys/socket.h>
#include <QTcpSocket>

struct GSock {
  static bool makeLocalHostAndPort(QTcpSocket* tcpSocket, QString localHost, quint16 localPort, int domain = AF_INET, int protocol = 0);
};
