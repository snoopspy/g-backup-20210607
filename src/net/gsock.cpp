#include "gsock.h"
#include <netinet/in.h>
#include <QDebug>
#include "gip.h"
#include "gip6.h"

bool GSock::makeLocalHostAndPort(QTcpSocket* tcpSocket, QString localHost, quint16 localPort, int domain, int protocol) {
  if (localHost == "" && localPort == 0) return true;

  // ------------------------------------------------------------------------
  // socket
  // ------------------------------------------------------------------------
  int sock = socket(domain, SOCK_STREAM, protocol);
  if (sock == -1) {
    qWarning() << "error in socket" << errno;
    return false;
  }

  // ------------------------------------------------------------------------
  // bind
  // ------------------------------------------------------------------------
  struct sockaddr_in addr;
  addr.sin_family = domain;
  addr.sin_port = htons(quint16(localPort));
  if (localHost == 0)
  {
    addr.sin_addr.s_addr = INADDR_ANY;
  } else
  {
    GIp ip = localHost;
    if (ip == 0)
    {
      qWarning() << QString("can not resolve host(%1)").arg(localHost);
      return false;
    }
    addr.sin_addr.s_addr = htonl(ip);
  }
  memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));

  int res = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
  if (res == -1)
  {
    qWarning() << QString("error in bind(%1:%2)").arg(localHost).arg(localPort);
    return false;
  }

  if (!tcpSocket->setSocketDescriptor(sock, QAbstractSocket::UnconnectedState)) {
    qWarning() <<  "setSocketDescriptor return false";
    return false;
  }
  return true;
}
