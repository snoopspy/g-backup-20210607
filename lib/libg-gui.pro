TEMPLATE = lib
CONFIG += staticlib
QT += widgets
CONFIG += G_BUILD
DEFINES += G_BUILD
include(../g.pri)
TARGET = $${G_NAME}
DESTDIR = $${PWD}/../bin
include(libg-files.pri)
