// ----- gilgil temp 2019.05.21 -----
/*
#include <GApp>

int main(int argc, char *argv[]) {
  GApp a(argc, argv);
  return a.exec_();
}
*/
// ----------------------------------

#include <GApp>
#include <GGraph>
#include "mygraph.h"

int main(int argc, char *argv[]) {
  GApp a(argc, argv);
  GGraph graph;
  MyFactory factory;
  return a.exec_(&graph, &factory);
}
