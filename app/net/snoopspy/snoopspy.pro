QT += widgets
include(../../../g.pri)
DESTDIR = $$PWD/../../../bin
SOURCES += *.cpp
HEADERS += *.h

android {
        deployment.files += $${PWD}/../../../bin/ssdemon
        deployment.path = /assets
        INSTALLS += deployment
}
