TEMPLATE = app
QT -= gui
CONFIG += G_BUILD
include(../g.pri)
TARGET = $${G_NAME}_test
LIBS *= -lpcap -lnetfilter_queue
android-g++: LIBS *= -lmnl -lnfnetlink
DESTDIR = $${PWD}/../bin
include(libg_files.pri)
DEFINES *= GTEST
LIBS *= -lgtest -lgtest_main -pthread
