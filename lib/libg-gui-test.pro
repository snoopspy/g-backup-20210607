TEMPLATE = app
QT += widgets
CONFIG += console
CONFIG += G_BUILD
DEFINES += G_BUILD
include(../g.pri)
TARGET = $${G_NAME}-test
linux: LIBS *= -lpcap -lnetfilter_queue
android: LIBS *= -lmnl -lnfnetlink
DESTDIR = $${PWD}/../bin
include(libg-files.pri)
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
