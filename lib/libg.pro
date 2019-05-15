TEMPLATE = lib
QT -= gui
CONFIG += G_BUILD
include(../g.pri)
TARGET = $${G_NAME}
win32: LIBS *= -lpacket -lwpcap -lws2_32
linux-g++: LIBS *= -lpcap -lnetfilter_queue
android-g++: LIBS *= -lmnl -lnfnetlink
DESTDIR = $${PWD}/../bin
include(libg_files.pri)
