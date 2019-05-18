TEMPLATE = subdirs
SUBDIRS += \
  enumintf \
  netclient \
  netserver \
  pcap_exam \
  pcap_findalldevs_test
win32 {
SUBDIRS += \
  getipforwardtable_test \
  getadaptersinfo_test
}
