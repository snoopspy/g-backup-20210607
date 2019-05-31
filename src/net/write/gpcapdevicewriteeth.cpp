#include "gpcapdevicewriteeth.h"

// ----------------------------------------------------------------------------
// GPcapDeviceWriteEth
// ----------------------------------------------------------------------------
bool GPcapDeviceWriteEth::doOpen() {
  if (GPcapDeviceWrite::doOpen())
    return false;

  if (dataLinkType_ != GPacket::Eth) {
    QString msg = QString("data link layer(%1) must be GPacket::Eth").arg(GPacket::dataLinkTypeToString(dataLinkType_));
    SET_ERR(GErr::FAIL, msg);
    return false;
  }

  smac_ = smacStr_;
  dmac_ = dmacStr_;
  return true;
}

bool GPcapDeviceWriteEth::doClose() {
  return GPcapDeviceWrite::doClose();
}

GPacket::Result GPcapDeviceWriteEth::write(GPacket* packet) {
  GPacket::Result res;
  switch (packet->dataLinkType_) {
    case GPacket::Eth:
      res = GPcapDeviceWrite::write(packet);
      break;
    case GPacket::Ip: {
      GEthHdr* ethHdr = PEthHdr(temp_);
      ethHdr->smac_ = smac_;
      ethHdr->dmac_ = dmac_;
      ethHdr->type_ = htons(type_);
      GBuf oldBuf = packet->buf_;
      GBuf newBuf(temp_, sizeof(GEthHdr) + oldBuf.size_);
      Q_ASSERT(newBuf.size_ <= MAXBUF);
      memcpy(temp_ + sizeof(GEthHdr), oldBuf.data_, oldBuf.size_);
      packet->buf_ = newBuf;
      res = GPcapDeviceWrite::write(packet);
      packet->buf_ = oldBuf;
      break;
    }
    case GPacket::Dot11:
    case GPacket::Null: {
      QString msg = QString("not supported data link type(%1)").arg(GPacket::dataLinkTypeToString(dataLinkType_));
      SET_ERR(GErr::NOT_SUPPORTED, msg);
      return GPacket::Fail;
    }
  }
  return res;
}
