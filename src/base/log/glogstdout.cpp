#include "glogstdout.h"

void GLogStdout::write(QString& msg) {
	printf("%s\n", qPrintable(msg));
}
