TEMPLATE = lib
QT -= gui
CONFIG += G_BUILD
include(../g.pri)
TARGET = $${G_NAME}
LIBS *= -lpcap -lnetfilter_queue
DESTDIR = $${PWD}/../bin
include(libg_files.pri)
