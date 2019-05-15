TEMPLATE = app
QT += widgets
CONFIG += G_BUILD
include(../g.pri)
TARGET = $${G_NAME}_test
win32: LIBS *= -lpacket -lwpcap -lws2_32
linux-g++: LIBS *= -lpcap -lnetfilter_queue
android-g++: LIBS *= -lmnl -lnfnetlink
DESTDIR = $${PWD}/../bin
include(libg_files.pri)
DEFINES *= GTEST
LIBS *= -lgtest -lgtest_main -pthread
