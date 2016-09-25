#include <unistd.h>
#include <iostream>
#include <QCoreApplication>
#include <QFile>
#include <GApp>
#include <GBase>
#include <GGraph>
#include <GJson>
#include <GPluginFactory>

struct Param {
  QString fileName_;

  bool parse(int argc, char* argv[]) {
    if (argc != 2) return false;
    fileName_ = argv[1];
    return true;
  }

  static void usage() {
    std::clog << "snoopspy console " << G_VERSION << std::endl;
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
    QJsonObject jo = GJson::loadFromFile(param->fileName_);
    graph_.propLoad(jo);
    QObject::connect(&graph_, &GStateObj::closed, this, &SsCon::terminate);
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
    qDebug() << "bef graph_.close"; // gilgil temp 2016.09.25
    return graph_.close();
    qDebug() << "aft graph_.close"; // gilgil temp 2016.09.25
  }

public slots:
  void terminate() {
    qDebug() << "terminate"; // gilgil temp 2016.09.25
    graph_.close();
    qDebug() << "bef call _exit"; // gilgil temp 2016.09.25
    _exit(EXIT_SUCCESS);
    qDebug() << "aft call _exit";
  }

public:
  GGraph graph_;
  GPluginFactory factory_;
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

  a.exec();

  qDebug() << "bef call ssCon.close()"; // gilgil temp 2016.09.25
  ssCon.close();
  qDebug() << "aft call ssCon.close()"; // gilgil temp 2016.09.25

  return EXIT_SUCCESS;
}

#include "sscon.moc"
