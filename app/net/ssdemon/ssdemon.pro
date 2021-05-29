QT -= qt gui
CONFIG += console
DESTDIR = $${PWD}/../../../bin
INCLUDEPATH += $${PWD}/../../../src/base/sys $${PWD}/../../../src/net/demon/
LIBS += -lpcap
SOURCES += *.cpp $${PWD}/../../../src/base/sys/gprocess.cpp $${PWD}/../../../src/net/demon/gdemon.cpp
HEADERS += *.h $${PWD}/../../../src/base/sys/gprocess.h $${PWD}/../../../src/net/demon/gdemon.h
