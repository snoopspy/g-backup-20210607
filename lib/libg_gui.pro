TEMPLATE = lib
QT += widgets
CONFIG += G_BUILD
include(../g.pri)
TARGET = $${G_NAME}
DESTDIR = $${PWD}/../bin
include(libg_files.pri)
