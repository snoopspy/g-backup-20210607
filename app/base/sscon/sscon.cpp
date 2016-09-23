#include <iostream>
#include <QCoreApplication>
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

int main(int argc, char* argv[]) {
  QCoreApplication a(argc, argv);
  GApp::init();

  Param param;
  if (!param.parse(argc, argv)) {
    Param::usage();
    return EXIT_FAILURE;
  }

  GGraph graph;
  GPluginFactory factory(&graph);
  factory.load("plugin/");

  QJsonObject jo = GJson::loadFromFile(param.fileName_);
  graph.propLoad(jo);
  if (!graph.open()) {
    std::clog << qPrintable(graph.err->msg()) << std::endl;
    return EXIT_FAILURE;
  }

  std::clog << "Press enter key to close\n";
  while (std::cin.get() != '\n');

  graph.close();

  return EXIT_SUCCESS;
}
