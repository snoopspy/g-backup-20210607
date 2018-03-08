#include "widget.h"

#include <QApplication>
#include <GApp>

Q_DECLARE_METATYPE(QSsl::SslProtocol)

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  GApp::init();
  Widget w;
  w.show();
  return a.exec();
}
