#include <iostream>
#include <string>
#include <QCoreApplication>
#include <GApp>
#include <GJson>
#include <GPcapDevice>
#include "net/pdu/gethpdu.h" // gilgil temp 2016.09.10

struct Obj : GObj {
  Q_OBJECT

public:
  Obj() : GObj(nullptr) {}

public slots:
  void captured(GPacket* packet) {
    GEthPdu* ethPdu = dynamic_cast<GEthPdu*>(packet->findFirst(GPdu::GEthPdu));
    if (ethPdu == nullptr) return;

    ETH_HDR* ethHdr = ethPdu->ethHdr_;
    qDebug() << QString("captured %1 > %2 (%3)").arg(
      QString(ethHdr->ether_shost), QString(ethHdr->ether_dhost), QString::number(ntohs(ethHdr->ether_type), 16));
  }
};

int main(int argc, char* argv[]) {
  QCoreApplication a(argc, argv);
  GApp::init();
  qDebug() << "1"; // gilgil temp 2016.09.12

  GPcapDevice device;

  QJsonObject jo = GJson::loadFromFile();
  jo["GPcapDevice"] >> device;
  jo["GPcapDevice"] << device;
  GJson::saveToFile(jo);

  Obj obj;
  QObject::connect(&device, SIGNAL(captured(GPacket*)), &obj, SLOT(captured(GPacket*)), Qt::DirectConnection);

  if (!device.open()) {
    qWarning() << device.err;
    return EXIT_FAILURE;
  }

  // std::cout << "Press enter key to close\n"; // gilgil temp 2016.09.12
  std::string s; std::cin >> s;

  qDebug() << "999"; // gilgil temp 2016.09.12
  return EXIT_SUCCESS;
}

#include "pcap_exam.moc"
