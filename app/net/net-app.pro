TEMPLATE = subdirs
SUBDIRS += \
	netclient \
	netinfo \
	netserver \
	pcap-exam \
	pcap-findalldevs-test \
	snoopspy \
	sscon
win32 {
SUBDIRS += \
	getipforwardtable-test \
	getadaptersinfo-test
}
