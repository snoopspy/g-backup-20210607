TEMPLATE = lib
win32: CONFIG += staticlib
QT += widgets
CONFIG *= G_BUILD
DEFINES *= G_BUILD
include(../g.pri)
TARGET = $${G_NAME}
linux-g++: LIBS *= -lpcap -lnetfilter_queue
android-g++: LIBS *= -lmnl -lnfnetlink
DESTDIR = $${PWD}/../bin
include(libg_files.pri)
