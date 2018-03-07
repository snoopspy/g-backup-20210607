#------------------------------------------------------------------------------
# QT
#------------------------------------------------------------------------------
CONFIG *= qt
QT *= network

#------------------------------------------------------------------------------
# c++11
#------------------------------------------------------------------------------
CONFIG *= c++11
QMAKE_LFLAGS *= -pthread

#------------------------------------------------------------------------------
# debug and release
#------------------------------------------------------------------------------
CONFIG(debug, debug|release) DEFINES *= _DEBUG
CONFIG(release, debug|release) DEFINES *= _RELEASE
DEFINES *= QT_MESSAGELOGCONTEXT

#------------------------------------------------------------------------------
# G_NAME
#------------------------------------------------------------------------------
G_NAME = g
android-g++:                  G_NAME = $${G_NAME}_android
CONFIG(qt): contains(QT, gui) G_NAME = $${G_NAME}_gui
CONFIG(debug, debug|release)  G_NAME = $${G_NAME}_d

#------------------------------------------------------------------------------
# G_DIR
#------------------------------------------------------------------------------
G_DIR = $${PWD}
INCLUDEPATH += $${G_DIR}/src
!CONFIG(G_BUILD) {
        PRE_TARGETDEPS *= $${G_DIR}/bin/lib$${G_NAME}.so
        LIBS *= -L$${G_DIR}/bin -l$${G_NAME}
}

#------------------------------------------------------------------------------
# mingw
#------------------------------------------------------------------------------
mingw: DEFINES *= __USE_MINGW_ANSI_STDIO=1

#------------------------------------------------------------------------------
# rpath
#------------------------------------------------------------------------------
QMAKE_RPATHDIR += .
QMAKE_RPATHDIR += $${PWD}/bin

#------------------------------------------------------------------------------
# pcap
#------------------------------------------------------------------------------
LIBS *= -lpcap

#------------------------------------------------------------------------------
# netfilter_queue
#------------------------------------------------------------------------------
LIBS *= -lnetfilter_queue

#------------------------------------------------------------------------------
# gstacktrace
#------------------------------------------------------------------------------
CONFIG(gstacktrace) {
	QMAKE_LFLAGS *= -rdynamic
}
