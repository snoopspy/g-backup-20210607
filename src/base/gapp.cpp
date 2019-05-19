#include "gapp.h"
#include <QString>

// ----------------------------------------------------------------------------
// GApp
// ----------------------------------------------------------------------------
#ifdef QT_GUI_LIB
GApp::GApp(int &argc, char **argv) : QApplication (argc, argv) {
#else
GApp::GApp(int &argc, char **argv) : QCoreApplication (argc, argv) {
#endif // QT_GUI_LIB
  init();
}

void GApp::init() {
  qSetMessagePattern("%{time yyyyMMdd h:mm:ss.zzz} %{threadid} [%{file}:%{line}] %{message}");
}
