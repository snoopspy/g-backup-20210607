#include "gapp.h"
#include <QDebug>
#include <GErr>

// ----------------------------------------------------------------------------
// GApp
// ----------------------------------------------------------------------------
GApp::GApp(int argc, char* argv[]) : QCoreApplication(argc, argv) {
  init();
}

GApp::~GApp() {

}

static void catchFunc(int signo) {
  GApp* app = GApp::instance();
  if (app == nullptr) return;
  emit app->signaled(signo);
}

void GApp::init() {
  qSetMessagePattern("%{time yyyyMMdd h:mm:ss.zzz} %{threadid} [%{file}:%{line}] %{message}");
  signal(SIGINT);
}

GApp* GApp::instance() {
  QCoreApplication* a = QCoreApplication::instance();
  GApp* app = dynamic_cast<GApp*>(a);
  if (app == nullptr) {
    qCritical() << "app it not GApp";
    return nullptr;
  }
  return app;
}

void GApp::signal(int signo) {
  __sighandler_t res = ::signal(signo, catchFunc);
  if (res == SIG_ERR) {
    qCritical() << QString("signal(%1) return SIGERR err=").arg(signo) << GLastErr();
  }
}
