#include <GApp>
#include "dept.h"

int main(int argc, char *argv[]) {
  GApp a(argc, argv);
  Dept dept;
  return a.execObj(&dept);
}
