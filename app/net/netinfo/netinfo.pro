QT += widgets network
include(../../../g.pri)
DESTDIR = $${PWD}/../../../bin
SOURCES += *.cpp
HEADERS += *.h
FORMS += widget.ui

android {
        deployment.files += $${PWD}/../../../bin/ssdemon
        deployment.path = /assets
        INSTALLS += deployment
}

