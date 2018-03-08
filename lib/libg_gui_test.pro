TEMPLATE = app
QT += widgets
CONFIG += G_BUILD
include(../g.pri)
TARGET = $${G_NAME}_test
LIBS *= -lpcap -lnetfilter_queue
DESTDIR = $${PWD}/../bin
include(libg_files.pri)
DEFINES *= GTEST
LIBS *= -lgtest -lgtest_main -pthread
