TEMPLATE = lib
QT += widgets
CONFIG += G_BUILD
DEFINES += G_BUILD
include(../g.pri)
TARGET = $${G_NAME}
linux: LIBS += -lpcap -lnetfilter_queue
android: LIBS += -lmnl -lnfnetlink
DESTDIR = $${PWD}/../bin
include(libg-files.pri)
