#include "gsscon.h"

int main(int argc, char* argv[]) {
	GSsCon sc(argc, argv);
	return sc.execFactory();
}
