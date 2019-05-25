SOURCES += \
  $${PWD}/../src/base/*.cpp \
  $${PWD}/../src/base/graph/*.cpp \
  $${PWD}/../src/base/other/*.cpp \
  $${PWD}/../src/base/prop/*.cpp \
  $${PWD}/../src/net/*.cpp \
  $${PWD}/../src/net/block/*.cpp \
  $${PWD}/../src/net/capture/*.cpp \
  $${PWD}/../src/net/delay/*.cpp \
  $${PWD}/../src/net/filter/*.cpp \
  $${PWD}/../src/net/flow/*.cpp \
  $${PWD}/../src/net/packet/*.cpp \
  $${PWD}/../src/net/pdu/*.cpp \
  $${PWD}/../src/net/process/*.cpp \
  $${PWD}/../src/net/write/*.cpp

HEADERS += \
  $${PWD}/../src/base/*.h \
  $${PWD}/../src/base/graph/*.h \
  $${PWD}/../src/base/other/*.h \
  $${PWD}/../src/base/prop/*.h \
  $${PWD}/../src/net/*.h \
  $${PWD}/../src/net/block/*.h \
  $${PWD}/../src/net/capture/*.h \
  $${PWD}/../src/net/delay/*.h \
  $${PWD}/../src/net/filter/*.h \
  $${PWD}/../src/net/flow/*.h \
  $${PWD}/../src/net/libnet/*.h \
  $${PWD}/../src/net/packet/*.h \
  $${PWD}/../src/net/pdu/*.h \
  $${PWD}/../src/net/process/*.h \
  $${PWD}/../src/net/write/*.h

linux {
  SOURCES += $${PWD}/../src/net/_linux/*.cpp
  HEADERS += $${PWD}/../src/net/_linux/*.h
}
win32 {
  SOURCES += $${PWD}/../src/net/_win32/*.cpp
  HEADERS += $${PWD}/../src/net/_win32/*.h
}

