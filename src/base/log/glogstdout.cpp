#include "glogstdout.h"

// ----------------------------------------------------------------------------
// GLogStdout
// ----------------------------------------------------------------------------
void GLogStdout::write(QString& msg) {
	printf("%s\n", qPrintable(msg));
	fflush(stdout);
}
