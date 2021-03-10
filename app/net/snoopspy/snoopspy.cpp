#include <GApp>
#include "myfactory.h"

int main(int argc, char *argv[]) {
	GApp a(argc, argv);
	MyFactory factory;
	return a.execFactory(&factory);
}
