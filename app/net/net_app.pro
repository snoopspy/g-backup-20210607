TEMPLATE = subdirs
SUBDIRS += \
  netclient \
  netinfo \
  netserver \
  pcap_exam \
  pcap_findalldevs_test
win32 {
SUBDIRS += \
  getipforwardtable_test \
  getadaptersinfo_test
}
