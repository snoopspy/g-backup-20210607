#------------------------------------------------------------------------------
# common
#------------------------------------------------------------------------------
CONFIG *= c++11 force_debug_info
linux: QMAKE_LFLAGS *= -pthread

#------------------------------------------------------------------------------
# debug and release
#------------------------------------------------------------------------------
CONFIG(debug, debug|release) DEFINES *= _DEBUG
CONFIG(release, debug|release) DEFINES *= _RELEASE
DEFINES *= QT_MESSAGELOGCONTEXT
DEFINES *= GILGIL_ANDROID_DEBUG # gilgil temp 2021.03.23 for android

#------------------------------------------------------------------------------
# G_NAME
#------------------------------------------------------------------------------
G_NAME = g
CONFIG(qt): contains(QT, gui) G_NAME = $${G_NAME}-gui
CONFIG(debug, debug|release) G_NAME = $${G_NAME}-d

#------------------------------------------------------------------------------
# G_DIR
#------------------------------------------------------------------------------
G_DIR = $${PWD}
INCLUDEPATH *= $${G_DIR}/src
!CONFIG(G_BUILD) {
	win32: PRE_TARGETDEPS *= $${G_DIR}/bin/$${G_NAME}.dll
	android: G_NAME = $${G_NAME}_armeabi-v7a
	linux: PRE_TARGETDEPS *= $${G_DIR}/bin/lib$${G_NAME}.so
	LIBS *= -L$${G_DIR}/bin -l$${G_NAME}
	ANDROID_EXTRA_LIBS *= $${G_DIR}/bin/lib$${G_NAME}.so
}

#------------------------------------------------------------------------------
# mingw
#------------------------------------------------------------------------------
mingw: DEFINES *= __USE_MINGW_ANSI_STDIO=1

#------------------------------------------------------------------------------
# rpath
#------------------------------------------------------------------------------
QMAKE_RPATHDIR *= . $${PWD}/bin

#------------------------------------------------------------------------------
# gstacktrace
#------------------------------------------------------------------------------
CONFIG(gstacktrace) {
	QMAKE_LFLAGS *= -rdynamic
}

#------------------------------------------------------------------------------
# path and link
#------------------------------------------------------------------------------
win32 {
	INCLUDEPATH *= $${PWD}/../npcap/Include
	LIBS *= -L$${PWD}/../npcap/Lib/x64
	LIBS *= -lwpcap -lpacket -lws2_32 -liphlpapi
}

#------------------------------------------------------------------------------
# resource
#------------------------------------------------------------------------------
CONFIG(qt): contains(QT, gui) {
	RESOURCES += $${PWD}/lib/libg-gui.qrc
}

#------------------------------------------------------------------------------
# android deploy files
#------------------------------------------------------------------------------
android {
	deployment.files += $${G_DIR}/bin/ssdemon
	deployment.path = /assets
	INSTALLS += deployment
}
