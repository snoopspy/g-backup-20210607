#include <GApp>
#include "mygraph.h"

int main(int argc, char *argv[]) {
	GApp a(argc, argv);
	MyFactory factory;
	return a.execFactory(&factory);
}
