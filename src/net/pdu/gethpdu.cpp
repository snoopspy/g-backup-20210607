#include "gethpdu.h"
#include <QDebug>

// ----------------------------------------------------------------------------
// GEthPdu
// ----------------------------------------------------------------------------
GEthPdu::GEthPdu(u_char* buf, size_t len) {
  if (len > ETHER_ADDR_LEN) {
    ethHdr_ = (ETH_HDR*)buf;
    len_ = ETHER_ADDR_LEN;
  } else {
    qWarning() << QString("invalid size(%)").arg(len);
    ethHdr_ = nullptr;
    len_ = 0;
  }
}
