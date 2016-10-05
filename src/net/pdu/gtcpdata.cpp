#include "gtcpdata.h"
#include "giphdr.h"
#include "gtcphdr.h"
#include "net/packet/gpacket.h"

// ----------------------------------------------------------------------------
// GTcpData
// ----------------------------------------------------------------------------
size_t GTcpData::size() {
  Q_ASSERT(data_ != nullptr);
  return size_;
}

GTcpData::GTcpData(u_char* data, size_t size) {
  data_ = data;
  size_ = size;
}

// ----------------------------------------------------------------------------
// GTcpDataParser
// ----------------------------------------------------------------------------
GPdu* GTcpDataParser::doParse(GPacket* packet, GPdu* prev) {
  Q_ASSERT(prev->pduType() == GPduType::Tcp);
  GTcpHdr* tcpHdr = (GTcpHdr*)prev;
  GPdus::reverse_iterator rit = packet->pdus_.rbegin();
  Q_ASSERT(*rit == prev); // tcpHdr
  rit++;
  Q_ASSERT(rit != packet->pdus_.rend());
  GPdu* l3Hdr = *rit;
  switch (l3Hdr->pduType()) {
    case GPduType::Ip: {
        GIpHdr* ipHdr = (GIpHdr*)l3Hdr;
        size_t size = ipHdr->len() - (ipHdr->hl() + tcpHdr->off()) * 4;
        if ((ssize_t)size <= 0)
          return nullptr;
        return new GTcpData(packet->parse_.data_, size);
      }
    case GPduType::Ip6:
      // TODO process ipv6 header // gilgil temp 2016.09.13
      return nullptr;
    default:
      qWarning() << QString("invalid pduType(%1)").arg((int)l3Hdr->pduType());
      return nullptr;
  }
}
