#include <iostream>
#include <string>
#include <QCoreApplication>
#include <GApp>
#include <GJson>
#include <GPcapDevice>
#include "net/pdu/gethpdu.h" // gilgil temp 2016.09.10
#include "net/pdu/gippdu.h" // gilgil temp 2016.09.10

struct Obj : GObj {
  Q_OBJECT

public:
  Obj() : GObj(nullptr) {}

public slots:
  void captured(GPacket* packet) {
    GEthPdu* eth = packet->findFirst<GEthPdu>();
    if (eth == nullptr) return;

    GIpPdu* ip = packet->findNext<GIpPdu>();
    if (ip == nullptr) return;

    char msg[256];
    sprintf(msg, "%s>%s ", (const char*)(eth->src()), (const char*)(eth->dst()));
    qDebug() << msg;
    sprintf(msg, "%s>%s ", qPrintable(QString(eth->src())), qPrintable(QString(eth->dst())));
    qDebug() << msg;
    sprintf(msg, "src %s ", (const char*)eth->src());
    qDebug() << msg;
    sprintf(msg, "dst %s ", (const char*)eth->dst());
    qDebug() << msg;
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
    qWarning() << device.err;
    return EXIT_FAILURE;
  }

  // std::cout << "Press enter key to close\n"; // gilgil temp 2016.09.12
  std::string s; std::cin >> s;

  return EXIT_SUCCESS;
}

#include "pcap_exam.moc"
