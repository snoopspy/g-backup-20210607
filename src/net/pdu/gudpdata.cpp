#include "gudpdata.h"
#include "giphdr.h"
#include "gudphdr.h"
#include "net/packet/gpacket.h"

// ----------------------------------------------------------------------------
// GUdpData
// ----------------------------------------------------------------------------
size_t GUdpData::size() {
  Q_ASSERT(data_ != nullptr);
  return size_;
}

GUdpData::GUdpData(u_char* data, size_t size) {
  data_ = data;
  size_ = size;
}

// ----------------------------------------------------------------------------
// GUdpDataParser
// ----------------------------------------------------------------------------
GPdu* GUdpDataParser::doParse(GPacket* packet) {
  GPdu* prev = packet->pdus_.last();
  Q_ASSERT(prev != nullptr && prev->pduType() == GPdu::Type::Udp);
  GUdpHdr* udpHdr = (GUdpHdr*)prev;

  GPdus& pdus = packet->pdus_;
  Q_ASSERT(pdus.rbegin() != pdus.rend()); // udpHdr
  Q_ASSERT(++pdus.rbegin() != pdus.rend()); // ipHdr
  GPdu* l3Hdr = *(++pdus.rbegin());
  switch (l3Hdr->pduType()) {
    case GPdu::Type::Ip: {
        GIpHdr* ipHdr = (GIpHdr*)l3Hdr;
        size_t size = ipHdr->len() - ipHdr->size() - udpHdr->size();
        if ((ssize_t)size <= 0)
          return nullptr;
        return new GUdpData(packet->parse_.data_, size);
      }
    case GPdu::Type::Ip6:
      // TODO process ipv6 header // gilgil temp 2016.09.13
      return nullptr;
    default:
      qWarning() << QString("invalid pduType(%1)").arg((int)l3Hdr->pduType());
      return nullptr;
  }
}
