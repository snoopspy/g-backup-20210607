#include <iostream>
#include <QCoreApplication>
#include <GApp>
#include <GJson>
#include <GPcapDevice>
#include "net/pdu/gethpdu.h"

struct Obj: GObj {
  Q_OBJECT

public slots:
  void captured(GPacket* packet) {
    GPdus::iterator it = packet->pdus_.find(GPdu::GEthPdu);
    GEthPdu* pdu = (GEthPdu*)*it;
    ETH_HDR* ethHdr = pdu->ethHdr_;
    qDebug() << "captured" << packet->pdus_.size() << QString(ethHdr->src) << QString(ethHdr->dst) << ethHdr->type;
  }
};

int main(int argc, char* argv[]) {
  QCoreApplication a(argc, argv);
  GApp::init();

  GPcapDevice device;

  QJsonObject jo = GJson::loadFromFile();
  jo["GPcapDevice"] >> device;

  Obj obj;
  QObject::connect(&device, SIGNAL(captured(GPacket*)), &obj, SLOT(captured(GPacket*)), Qt::DirectConnection);

  if (!device.open()) {
    qWarning() << device.err;
    return EXIT_FAILURE;
  }

  std::cout << "Press enter key to close\n";
  std::cin.get();

  jo["GPcapDevice"] << device;
  GJson::saveToFile(jo);

  return EXIT_SUCCESS;
}

#include "pcap_exam.moc"
