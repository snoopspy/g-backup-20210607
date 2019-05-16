#include <iostream>
#include <QCoreApplication>
#include <QFile>
#include <GApp>
#include <GBase>
#include <GGraph>
#include <GJson>
#include <GPluginFactory>
#include <GSignal>

struct Param {
  QString fileName_;

  bool parse(int argc, char* argv[]) {
    if (argc != 2) return false;
    fileName_ = argv[1];
    return true;
  }

  static void usage() {
    std::clog << "snoopspy console " << G::Version << std::endl;
    std::clog << "Copyright (c) Gilbert Lee All rights reserved\n";
    std::clog << "\n";
    std::clog << "usage : sscon <filename>\n";
    std::clog << "\n";
    std::clog << "example : sscon ss/test.ss\n";
    std::clog << "\n";
  }
};

struct SsCon : GStateObj {
  Q_OBJECT

public:
  SsCon(Param* param) : GStateObj(nullptr), factory_(&graph_) {
    factory_.load("plugin/");

    GSignal& signal = GSignal::instance();
    signal.setup(SIGINT);
    QObject::connect(&signal, &GSignal::signaled, this, &SsCon::processSignal);

    QJsonObject jo = GJson::loadFromFile(param->fileName_);
    graph_.propLoad(jo);
    QObject::connect(&graph_, &GStateObj::closed, this, &SsCon::processClose);
  }

  ~SsCon() override {
    close();
  }

protected:
  bool doOpen() override {
    bool res = graph_.open();
    if (!res)
      err = graph_.err;
    return res;
  }

  bool doClose() override {
    qDebug() << "bef call graph_.close"; // gilgil temp 2016.09.25
    bool res = graph_.close();
    qDebug() << "aft call graph_.close"; // gilgil temp 2016.09.25
    return res;
  }

public:
  GGraph graph_;
  GPluginFactory factory_;

public slots:
  void processClose() {
    qDebug() << "processClose"; // gilgil temp 2016.09.25
    qDebug() << "bef call QCoreApplication::quit()"; // gilgil temp 2016.09.25
    QCoreApplication::quit();
    qDebug() << "aft call QCoreApplication::quit()"; // gilgil temp 2016.09.25
  }

  void processSignal(int signo) {
    qDebug() << "beg processSignal" << signo; // gilgil temp 2016.09.25
    if (signo == SIGINT) {
      qDebug() << "bef call graph_.close()" << signo; // gilgil temp 2016.09.25
      graph_.close();
      qDebug() << "aft call graph_.close()" << signo; // gilgil temp 2016.09.25
    }
    qDebug() << "end processSignal" << signo; // gilgil temp 2016.09.25
  }
};

int main(int argc, char* argv[]) {
  QCoreApplication a(argc, argv);
  GApp::init();

  Param param;
  if (!param.parse(argc, argv)) {
    Param::usage();
    return EXIT_FAILURE;
  }
  if (!QFile::exists(param.fileName_)) {
    std::clog << "no such file " << qPrintable(param.fileName_) << std::endl;
    return EXIT_FAILURE;
  }

  SsCon ssCon(&param);
  if (!ssCon.open()) {
    std::clog << qPrintable(ssCon.err->msg()) << std::endl;
    return EXIT_FAILURE;
  }

  qDebug() << "bef call a.exec()";
  int res = a.exec();
  qDebug() << "aft call a.exec()" << res;
  return res;
}

#include "sscon.moc"
