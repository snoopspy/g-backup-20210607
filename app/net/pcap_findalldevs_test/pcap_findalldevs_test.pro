CONFIG -= qt
CONFIG += console
SOURCES += *.cpp
#------------------------------------------------------------------------------
# pcap
#------------------------------------------------------------------------------
win32 {
  INCLUDEPATH *= $${PWD}/../../../../npcap/Include
  contains(QMAKE_TARGET.arch, x86_64) { # 64 bit
    LIBS *= -L$${PWD}/../../../../npcap/Lib/x64
  } else { # 32 bit
    LIBS *= -L$${PWD}/../../../../npcap/Lib
  }
  LIBS *= -lpacket -lwpcap -lws2_32
}
linux : LIBS += -lpcap
DESTDIR = $${PWD}/../../../bin
