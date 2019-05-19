#include <iostream>
#include <QCoreApplication>
#include <QDebug>
#include <GApp>
#include <GEthPacket>
#include <GIpPacket>
#include <GJson>
#include <GPcapDevice>

struct Obj : QObject {
  Q_OBJECT

public slots:
  void captured(GPacket* packet) {
    GEthPacket* ethPacket = nullptr;
    GIpPacket* ipPacket;
    switch (packet->dataLinkType_) {
      case GPacket::Eth:
        ethPacket = PEthPacket(packet);
        ipPacket = PEthPacket(packet);
        break;
      case GPacket::Ip:
        ipPacket = PIpPacket(packet);
        break;
      case GPacket::Dot11:
        return;
      case GPacket::Null:
        return;
    }

    QString smac;
    QString dmac;
    if (ethPacket != nullptr) {
      GEthHdr* ethHdr = ethPacket->ethHdr_;
      if (ethHdr == nullptr) return;
      smac = QString(ethHdr->smac());
      dmac = QString(ethHdr->dmac());
    }

    GIpHdr* ipHdr = ipPacket->ipHdr_;
    if (ipHdr == nullptr) return;
    QString sip = QString(ipHdr->sip());
    QString dip = QString(ipHdr->dip());

    QString proto, sport, dport;
    GTcpHdr* tcpHdr = ipPacket->tcpHdr_;
    if (tcpHdr != nullptr) {
      proto = "tcp";
      sport = QString::number(tcpHdr->sport());
      dport = QString::number(tcpHdr->dport());
    } else {
      GUdpHdr* udpHdr = ipPacket->udpHdr_;
      if (udpHdr != nullptr) {
        proto = "udp";
        sport = QString::number(udpHdr->sport());
        dport = QString::number(udpHdr->dport());
      } else {
        return;
      }
    }

    static int count = 0;
    QString msg = QString("%1 %2 %3 > %4 %5:%6 > %7:%8\n").
        arg(++count).arg(proto, smac, dmac, sip, sport, dip, dport);
    std::clog << qPrintable(msg);
  }

  void processClose() {
    qDebug() << "processClose";
    QCoreApplication::quit();
  }
};

int main(int argc, char* argv[]) {
  GApp a(argc, argv);

  GPcapDevice device;
  QJsonObject jo = GJson::loadFromFile();
  jo["device"] >> device;
  jo["device"] << device;
  GJson::saveToFile(jo);

  Obj obj;
  QObject::connect(&device, &GPcapDevice::captured, &obj, &Obj::captured, Qt::DirectConnection);
  QObject::connect(&device, &GPcapDevice::closed,   &obj, &Obj::processClose);

  if (!device.open()) {
    std::clog << device.err;
    return EXIT_FAILURE;
  }

  return a.exec();
}

#include "pcap_exam.moc"
