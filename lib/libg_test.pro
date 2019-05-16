TEMPLATE = app
QT -= gui
CONFIG += G_BUILD
include(../g.pri)
TARGET = $${G_NAME}_test
win32: LIBS *= -lpacket -lwpcap -lws2_32
linux: LIBS *= -lpcap -lnetfilter_queue
android-g++: LIBS *= -lmnl -lnfnetlink
DESTDIR = $${PWD}/../bin
include(libg_files.pri)
DEFINES *= GTEST
win32 {
  INCLUDEPATH *= $${PWD}/../../gtest/include
  LIBS *= -L$${PWD}/../../gtest/lib
  CONFIG(debug, debug|release) LIBS *= -lgtest_maind -lgtestd
  CONFIG(release, debug|release) LIBS *= -lgtest_main -lgtest
}
linux {
  LIBS *= -lgtest_main -lgtest
}
