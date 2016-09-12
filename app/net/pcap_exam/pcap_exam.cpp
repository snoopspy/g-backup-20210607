#include <iostream>
#include <QCoreApplication>
#include <GApp>
#include <GEthHdr>
#include <GIpHdr>
#include <GJson>
#include <GPcapDevice>

struct Obj : GObj {
  Q_OBJECT

public:
  Obj() : GObj(nullptr) {}

public slots:
  void captured(GPacket* packet) {
    GEthHdr* ethHdr = packet->findFirst<GEthHdr>();
    if (ethHdr == nullptr) return;

    GIpHdr* ipHdr = packet->findNext<GIpHdr>();
    if (ipHdr == nullptr) return;

    QString smac = (QString)ethHdr->smac();
    QString dmac = (QString)ethHdr->dmac();
    QString sip  = (QString)ipHdr->sip();
    QString dip  = (QString)ipHdr->dip();

    QString msg = QString("%1 > %2 %3 > %4\n").arg(smac, dmac, sip, dip);
    std::clog << qPrintable(msg);
  }
};

int main(int argc, char* argv[]) {
  QCoreApplication a(argc, argv);
  GApp::init();

  GPcapDevice device;

  QJsonObject jo = GJson::loadFromFile();
  jo["GPcapDevice"] >> device;
  jo["GPcapDevice"] << device;
  GJson::saveToFile(jo);

  Obj obj;
  QObject::connect(&device, SIGNAL(captured(GPacket*)), &obj, SLOT(captured(GPacket*)), Qt::DirectConnection);

  if (!device.open()) {
    std::clog << device.err;
    return EXIT_FAILURE;
  }

  std::clog << "Press enter key to close\n";
  while (std::cin.get() != '\n');

  return EXIT_SUCCESS;
}

#include "pcap_exam.moc"
