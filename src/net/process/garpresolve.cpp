#include "garpresolve.h"
#include "net/packet/gpacketcast.h"
#include <QElapsedTimer>

// ----------------------------------------------------------------------------
// GArpResolve
// ----------------------------------------------------------------------------
bool GArpResolve::waitResolve(GPcapDevice* pcapDevice, unsigned long timeout) {
  if (arpTable_.ok())
    return true;

  if (!pcapDevice->active()) {
    QString msg = QString("not opened state %1").arg(pcapDevice->metaObject()->className());
    SET_ERR(GErr::FAIL, msg);
    return false;
  }

  GPacket::DataLinkType dataLinkType = pcapDevice->dataLinkType();
  if (dataLinkType != GPacket::Eth) {
    QString msg = QString("invalid dataLinkType %1").arg(int(dataLinkType));
    SET_ERR(GErr::FAIL, msg);
    return false;
  }

  GNetIntf* intf = GNetIntf::all().findByName(pcapDevice->devName_);
  if (intf == nullptr) {
    QString msg = QString("can not find intf for %1").arg(pcapDevice->devName_);
    SET_ERR(GErr::FAIL, msg);
    return false;
  }

  if (!sendQueries(pcapDevice, intf)) return false;
  QElapsedTimer timer; timer.start();
  qint64 first = timer.elapsed();
  qint64 last = first;

  while (true) {
    if (arpTable_.ok())
      return true;

    qint64 now = timer.elapsed();
    if (now - last >= 1000) {
      if (!sendQueries(pcapDevice, intf)) return false;
      last = now;
    }

    if (now - first > qint64(timeout)) {
      QString msg = "can not resolve all ip";
      for (ArpTable::iterator it = arpTable_.begin(); it != arpTable_.end(); it++) {
        GIp ip = it.key();
        msg += " ";
        msg += QString(ip);
      }
      SET_ERR(GErr::FAIL, msg);
      return false;
    }

    GEthPacket packet;
    GPacket::Result res = pcapDevice->read(&packet);
    switch (res) {
      case GPacket::Eof:
        SET_ERR(GErr::FAIL, "pcapDevice->read return GPacket::Eof");
        return false;
      case GPacket::Fail:
        SET_ERR(GErr::FAIL, "pcapDevice->read return GPacket::Fail");
        return false;
      case GPacket::TimeOut:
        continue;
      case GPacket::Ok:
        break;
    }
    packet.parse();

    GArpHdr* arpHdr = packet.arpHdr_;
    if (arpHdr == nullptr) continue;
    if (arpHdr->op() != GArpHdr::REPLY) continue;

    GIp sip = arpHdr->sip();
    GMac smac = arpHdr->smac();
    ArpTable::iterator it = arpTable_.find(sip);
    if (it != arpTable_.end()) {
      it.value() = smac;
      if (arpTable_.ok())
        return true;
    }
  }
}

bool GArpResolve::sendQueries(GPcapDevice* pcapDevice, GNetIntf* intf) {
  GEthArpHdr query;
  query.ethHdr_.dmac_ = GMac::broadcastMac();
  query.ethHdr_.smac_ = intf->mac();
  query.ethHdr_.type_ = htons(GEthHdr::ARP);

  query.arpHdr_.hrd_ = htons(GArpHdr::ETHER);
  query.arpHdr_.pro_ = htons(GEthHdr::IP4);
  query.arpHdr_.hln_ = sizeof(GMac);
  query.arpHdr_.pln_ = sizeof(GIp);
  query.arpHdr_.op_ = htons(GArpHdr::REQUEST);
  query.arpHdr_.smac_ = intf->mac();
  query.arpHdr_.sip_ = htonl(intf->ip());
  query.arpHdr_.tmac_ = GMac::cleanMac();
  GBuf queryBuf(pbyte(&query), sizeof(query));

  for (ArpTable::iterator it = arpTable_.begin(); it != arpTable_.end(); it++) {
    GIp ip = it.key();
    GMac mac = it.value();
    if (mac.isClean()) {
      query.arpHdr_.tip_ = htonl(ip);
      GPacket::Result res = pcapDevice->write(queryBuf);
      if (res != GPacket::Ok) {
        qCritical() << "pcapDevice->write return GPacket::Fail";
        return false;
      }
    }
  }
  return true;
}

void GArpResolve::resolve(GPacket* packet) {
  GEthPacket* ethPacket = GPacketCast::toEth(packet);
  if (ethPacket == nullptr) return;

  GArpHdr* arpHdr = ethPacket->arpHdr_;
  if (arpHdr == nullptr) return;

  emit resolved(packet);
}

// ----------------------------------------------------------------------------
// GTEST
// ----------------------------------------------------------------------------
#ifdef GTEST
#include <gtest/gtest.h>

#include "net/capture/gsyncpcapdevice.h"
TEST(GArpResolve, resolveTest) {
  GSyncPcapDevice device;

  ASSERT_TRUE(device.open());

  QString devName = device.devName_;
  ASSERT_NE(devName, "");

  GNetIntf* intf = GNetIntf::all().findByName(devName);
  ASSERT_TRUE(intf != nullptr);

  GIp ip = intf->gateway();
  ASSERT_NE(ip, 0);

  GArpResolve ar;
  ar.arpTable_[ip] = GMac::cleanMac();

  ASSERT_TRUE(ar.waitResolve(&device, 10000000)); // gilgil temp 2019.05.27
  GMac mac = ar.arpTable_[ip];

  qDebug() << QString("ip:%1 mac:%2").arg(QString(ip), QString(mac));
}

#endif // GTEST
