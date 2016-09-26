#include <unistd.h>
#include <iostream>
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

    QObject::connect(&graph_, &GStateObj::closed, this, &SsCon::processClose);
    QObject::connect(GApp::instance(), &GApp::signaled, this, &SsCon::processSignal);
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

public:
  GGraph graph_;
  GPluginFactory factory_;

public slots:
  void processClose() {
    qDebug() << "processClose"; // gilgil temp 2016.09.25
    qDebug() << "bef GApp::instance()->quit()"; // gilgil temp 2016.09.25
    GApp::instance()->quit();
    qDebug() << "aft GApp::instance()->quit()";
  }

  void processSignal(int signo) {
    qDebug() << "beg processSignal" << signo; // gilgil temp 2016.09.25
    if (signo == SIGINT) {
      graph_.close();
    }
    qDebug() << "end processSignal" << signo; // gilgil temp 2016.09.25
  }
};

int main(int argc, char* argv[]) {
  GApp a(argc, argv);

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

  return a.exec();
}

#include "sscon.moc"
