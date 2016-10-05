#include <iostream>
#include <QCoreApplication>
#include <GApp>
#include <GEthHdr>
#include <GIpHdr>
#include <GJson>
#include <GPcapDevice>
#include <GTcpHdr>
#include <GUdpHdr>

struct Obj : QObject {
  Q_OBJECT

public slots:
  void captured(GPacket* packet) {
    GPdus& pdus = packet->pdus_;

    GEthHdr* ethHdr = pdus.findFirst<GEthHdr>();
    if (ethHdr == nullptr) return;
    QString smac  = (QString)ethHdr->smac();
    QString dmac  = (QString)ethHdr->dmac();

    GIpHdr* ipHdr = pdus.findNext<GIpHdr>();
    if (ipHdr == nullptr) return;
    QString sip   = (QString)ipHdr->sip();
    QString dip   = (QString)ipHdr->dip();

    QString proto, sport, dport;
    GTcpHdr* tcpHdr = pdus.findNext<GTcpHdr>();
    if (tcpHdr != nullptr) {
      proto = "tcp";
      sport = QString::number(tcpHdr->sport());
      dport = QString::number(tcpHdr->dport());
    } else {
      GUdpHdr* udpHdr = pdus.findNext<GUdpHdr>();
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
  QCoreApplication a(argc, argv);
  GApp::init();

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
