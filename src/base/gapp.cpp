#include "gapp.h"
#include <QString>

// ----------------------------------------------------------------------------
// GApp
// ----------------------------------------------------------------------------
void GApp::init() {
  qSetMessagePattern("%{time yyyyMMdd h:mm:ss.zzz} %{threadid} [%{file}:%{line}] %{message}");
}
