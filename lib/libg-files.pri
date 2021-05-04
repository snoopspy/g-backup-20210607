SOURCES += \
	$${PWD}/../src/base/gapp.cpp \
	$${PWD}/../src/base/gbase.cpp \
	$${PWD}/../src/base/gbuf.cpp \
	$${PWD}/../src/base/gerr.cpp \
	$${PWD}/../src/base/gjson.cpp \
	$${PWD}/../src/base/gobj.cpp \
	$${PWD}/../src/base/graph/gg-arrow.cpp \
	$${PWD}/../src/base/graph/gg-scene.cpp \
	$${PWD}/../src/base/graph/gg-signalslotform.cpp \
	$${PWD}/../src/base/graph/gg-text.cpp \
	$${PWD}/../src/base/graph/ggraph.cpp \
	$${PWD}/../src/base/graph/ggraphwidget.cpp \
	$${PWD}/../src/base/graph/gpluginfactory.cpp \
	$${PWD}/../src/base/grwpool.cpp \
	$${PWD}/../src/base/gsscon.cpp \
	$${PWD}/../src/base/gstateobj.cpp \
	$${PWD}/../src/base/log/glog.cpp \
	$${PWD}/../src/base/log/glogdbwin32.cpp \
	$${PWD}/../src/base/log/glogfile.cpp \
	$${PWD}/../src/base/log/glogmanager.cpp \
	$${PWD}/../src/base/log/glogstderr.cpp \
	$${PWD}/../src/base/log/glogstdout.cpp \
	$${PWD}/../src/base/log/glogudp.cpp \
	$${PWD}/../src/base/other/gcommand.cpp \
	$${PWD}/../src/base/prop/gprop.cpp \
	$${PWD}/../src/base/prop/gpropitem-bool.cpp \
	$${PWD}/../src/base/prop/gpropitem-char.cpp \
	$${PWD}/../src/base/prop/gpropitem-checkbox.cpp \
	$${PWD}/../src/base/prop/gpropitem-combobox.cpp \
	$${PWD}/../src/base/prop/gpropitem-dirpath.cpp \
	$${PWD}/../src/base/prop/gpropitem-enum.cpp \
	$${PWD}/../src/base/prop/gpropitem-filepath.cpp \
	$${PWD}/../src/base/prop/gpropitem-interface.cpp \
	$${PWD}/../src/base/prop/gpropitem-lineedit.cpp \
	$${PWD}/../src/base/prop/gpropitem-objptr.cpp \
	$${PWD}/../src/base/prop/gpropitem-objref.cpp \
	$${PWD}/../src/base/prop/gpropitem-objrefarrayptr.cpp \
	$${PWD}/../src/base/prop/gpropitem-plaintextedit.cpp \
	$${PWD}/../src/base/prop/gpropitem-stringlist.cpp \
	$${PWD}/../src/base/prop/gpropitem-toolbutton.cpp \
	$${PWD}/../src/base/prop/gpropitem-unknowntype.cpp \
	$${PWD}/../src/base/prop/gpropitem-variant.cpp \
	$${PWD}/../src/base/prop/gpropitem-variantselect.cpp \
	$${PWD}/../src/base/prop/gpropitem-widget.cpp \
	$${PWD}/../src/base/prop/gpropitem.cpp \
	$${PWD}/../src/base/prop/gpropwidget.cpp \
	$${PWD}/../src/base/sys/gsignal.cpp \
	$${PWD}/../src/base/sys/gthread.cpp \
	$${PWD}/../src/base/sys/gwaitevent.cpp \
	$${PWD}/../src/net/block/gblock.cpp \
	$${PWD}/../src/net/block/gtcpblock.cpp \
	$${PWD}/../src/net/capture/garpspoof.cpp \
	$${PWD}/../src/net/capture/gcapture.cpp \
	$${PWD}/../src/net/capture/gnetfilter.cpp \
	$${PWD}/../src/net/capture/gpcapcapture.cpp \
	$${PWD}/../src/net/capture/gpcapdevice.cpp \
	$${PWD}/../src/net/capture/gpcapfile.cpp \
	$${PWD}/../src/net/capture/gremote.cpp \
	$${PWD}/../src/net/capture/gremotepcapdevice.cpp \
	$${PWD}/../src/net/capture/gsyncnetfilter.cpp \
	$${PWD}/../src/net/capture/gsyncpcapdevice.cpp \
	$${PWD}/../src/net/capture/gsyncpcapfile.cpp \
	$${PWD}/../src/net/capture/gwindivert.cpp \
	$${PWD}/../src/net/convert/gconverteth.cpp \
	$${PWD}/../src/net/convert/gconvertethautomac.cpp \
	$${PWD}/../src/net/convert/gconvertip.cpp \
	$${PWD}/../src/net/delay/gdelay.cpp \
	$${PWD}/../src/net/delay/gsyncdelay.cpp \
	$${PWD}/../src/net/delay/gtimestampsyncdelay.cpp \
	$${PWD}/../src/net/extract/gextract.cpp \
	$${PWD}/../src/net/extract/ghttpextract.cpp \
	$${PWD}/../src/net/filter/gbpfilter.cpp \
	$${PWD}/../src/net/flow/gflow.cpp \
	$${PWD}/../src/net/flow/gflowkey.cpp \
	$${PWD}/../src/net/flow/gflowmgr.cpp \
	$${PWD}/../src/net/flow/gipflowmgr.cpp \
	$${PWD}/../src/net/flow/gtcpflowmgr.cpp \
	$${PWD}/../src/net/flow/gudpflowmgr.cpp \
	$${PWD}/../src/net/gatm.cpp \
	$${PWD}/../src/net/ginterface.cpp \
	$${PWD}/../src/net/gip.cpp \
	$${PWD}/../src/net/gip6.cpp \
	$${PWD}/../src/net/gmac.cpp \
	$${PWD}/../src/net/gnet.cpp \
	$${PWD}/../src/net/gnetinfo.cpp \
	$${PWD}/../src/net/grtm.cpp \
	$${PWD}/../src/net/demon/gdemon.cpp \
	$${PWD}/../src/net/demon/gdemonclient.cpp \
	$${PWD}/../src/net/demon/gtrace.cpp \
	$${PWD}/../src/net/packet/gdot11packet.cpp \
	$${PWD}/../src/net/packet/gethpacket.cpp \
	$${PWD}/../src/net/packet/gippacket.cpp \
	$${PWD}/../src/net/packet/gnullpacket.cpp \
	$${PWD}/../src/net/packet/gpacket.cpp \
	$${PWD}/../src/net/pdu/garphdr.cpp \
	$${PWD}/../src/net/pdu/gdnshdr.cpp \
	$${PWD}/../src/net/pdu/gdnsinfo.cpp \
	$${PWD}/../src/net/pdu/gethhdr.cpp \
	$${PWD}/../src/net/pdu/gicmphdr.cpp \
	$${PWD}/../src/net/pdu/gip6hdr.cpp \
	$${PWD}/../src/net/pdu/giphdr.cpp \
	$${PWD}/../src/net/pdu/gpdu.cpp \
	$${PWD}/../src/net/pdu/gtcphdr.cpp \
	$${PWD}/../src/net/pdu/gudphdr.cpp \
	$${PWD}/../src/net/process/gclienthellosplit.cpp \
	$${PWD}/../src/net/process/gcorrectchecksum.cpp \
	$${PWD}/../src/net/process/gdnsprocessor.cpp \
	$${PWD}/../src/net/process/gflowmgrdebugger.cpp \
	$${PWD}/../src/net/process/gpacketdebugger.cpp \
	$${PWD}/../src/net/write/gpcapdevicewrite.cpp \
	$${PWD}/../src/net/write/gpcapfilewrite.cpp \
	$${PWD}/../src/net/write/gpcapwrite.cpp \
	$${PWD}/../src/net/write/grawipsocketwrite.cpp \
	$${PWD}/../src/net/write/gwrite.cpp

HEADERS += \
	$${PWD}/../src/base/gapp.h \
	$${PWD}/../src/base/gbase.h \
	$${PWD}/../src/base/gbuf.h \
	$${PWD}/../src/base/gerr.h \
	$${PWD}/../src/base/gerrcategory.h \
	$${PWD}/../src/base/gjson.h \
	$${PWD}/../src/base/gobj.h \
	$${PWD}/../src/base/graph/gg-arrow.h \
	$${PWD}/../src/base/graph/gg-scene.h \
	$${PWD}/../src/base/graph/gg-signalslotform.h \
	$${PWD}/../src/base/graph/gg-text.h \
	$${PWD}/../src/base/graph/ggraph.h \
	$${PWD}/../src/base/graph/ggraphwidget.h \
	$${PWD}/../src/base/graph/gpluginfactory.h \
	$${PWD}/../src/base/grwpool.h \
	$${PWD}/../src/base/gsscon.h \
	$${PWD}/../src/base/gstateobj.h \
	$${PWD}/../src/base/log/glog.h \
	$${PWD}/../src/base/log/glogdbwin32.h \
	$${PWD}/../src/base/log/glogfile.h \
	$${PWD}/../src/base/log/glogmanager.h \
	$${PWD}/../src/base/log/glogstderr.h \
	$${PWD}/../src/base/log/glogstdout.h \
	$${PWD}/../src/base/log/glogudp.h \
	$${PWD}/../src/base/other/gcommand.h \
	$${PWD}/../src/base/prop/gprop.h \
	$${PWD}/../src/base/prop/gpropitem-bool.h \
	$${PWD}/../src/base/prop/gpropitem-char.h \
	$${PWD}/../src/base/prop/gpropitem-checkbox.h \
	$${PWD}/../src/base/prop/gpropitem-combobox.h \
	$${PWD}/../src/base/prop/gpropitem-dirpath.h \
	$${PWD}/../src/base/prop/gpropitem-enum.h \
	$${PWD}/../src/base/prop/gpropitem-filepath.h \
	$${PWD}/../src/base/prop/gpropitem-interface.h \
	$${PWD}/../src/base/prop/gpropitem-lineedit.h \
	$${PWD}/../src/base/prop/gpropitem-objptr.h \
	$${PWD}/../src/base/prop/gpropitem-objref.h \
	$${PWD}/../src/base/prop/gpropitem-objrefarrayptr.h \
	$${PWD}/../src/base/prop/gpropitem-plaintextedit.h \
	$${PWD}/../src/base/prop/gpropitem-stringlist.h \
	$${PWD}/../src/base/prop/gpropitem-toolbutton.h \
	$${PWD}/../src/base/prop/gpropitem-unknowntype.h \
	$${PWD}/../src/base/prop/gpropitem-variant.h \
	$${PWD}/../src/base/prop/gpropitem-variantselect.h \
	$${PWD}/../src/base/prop/gpropitem-widget.h \
	$${PWD}/../src/base/prop/gpropitem.h \
	$${PWD}/../src/base/prop/gpropwidget.h \
	$${PWD}/../src/base/sys/gsignal.h \
	$${PWD}/../src/base/sys/gthread.h \
	$${PWD}/../src/base/sys/gwaitevent.h \
	$${PWD}/../src/net/block/gblock.h \
	$${PWD}/../src/net/block/gtcpblock.h \
	$${PWD}/../src/net/capture/garpspoof.h \
	$${PWD}/../src/net/capture/gcapture.h \
	$${PWD}/../src/net/capture/gnetfilter.h \
	$${PWD}/../src/net/capture/gpcapcapture.h \
	$${PWD}/../src/net/capture/gpcapdevice.h \
	$${PWD}/../src/net/capture/gpcapfile.h \
	$${PWD}/../src/net/capture/gremote.h \
	$${PWD}/../src/net/capture/gremotepcapdevice.h \
	$${PWD}/../src/net/capture/gsyncnetfilter.h \
	$${PWD}/../src/net/capture/gsyncpcapdevice.h \
	$${PWD}/../src/net/capture/gsyncpcapfile.h \
	$${PWD}/../src/net/capture/gwindivert.h \
	$${PWD}/../src/net/convert/gconverteth.h \
	$${PWD}/../src/net/convert/gconvertethautomac.h \
	$${PWD}/../src/net/convert/gconvertip.h \
	$${PWD}/../src/net/delay/gdelay.h \
	$${PWD}/../src/net/delay/gsyncdelay.h \
	$${PWD}/../src/net/delay/gtimestampsyncdelay.h \
	$${PWD}/../src/net/extract/gextract.h \
	$${PWD}/../src/net/extract/ghttpextract.h \
	$${PWD}/../src/net/filter/gbpfilter.h \
	$${PWD}/../src/net/flow/gflow.h \
	$${PWD}/../src/net/flow/gflowkey.h \
	$${PWD}/../src/net/flow/gflowmgr.h \
	$${PWD}/../src/net/flow/gipflowmgr.h \
	$${PWD}/../src/net/flow/gtcpflowmgr.h \
	$${PWD}/../src/net/flow/gudpflowmgr.h \
	$${PWD}/../src/net/gatm.h \
	$${PWD}/../src/net/ginterface.h \
	$${PWD}/../src/net/gip.h \
	$${PWD}/../src/net/gip6.h \
	$${PWD}/../src/net/gmac.h \
	$${PWD}/../src/net/gnet.h \
	$${PWD}/../src/net/gnetinfo.h \
	$${PWD}/../src/net/grtm.h \
	$${PWD}/../src/net/demon/gdemon.h \
	$${PWD}/../src/net/demon/gdemonclient.h \
	$${PWD}/../src/net/demon/gtrace.h \
	$${PWD}/../src/net/packet/gdot11packet.h \
	$${PWD}/../src/net/packet/gethpacket.h \
	$${PWD}/../src/net/packet/gippacket.h \
	$${PWD}/../src/net/packet/gnullpacket.h \
	$${PWD}/../src/net/packet/gpacket.h \
	$${PWD}/../src/net/pdu/garphdr.h \
	$${PWD}/../src/net/pdu/gdnshdr.h \
	$${PWD}/../src/net/pdu/gdnsinfo.h \
	$${PWD}/../src/net/pdu/gethhdr.h \
	$${PWD}/../src/net/pdu/gicmphdr.h \
	$${PWD}/../src/net/pdu/gip6hdr.h \
	$${PWD}/../src/net/pdu/giphdr.h \
	$${PWD}/../src/net/pdu/gpdu.h \
	$${PWD}/../src/net/pdu/gtcphdr.h \
	$${PWD}/../src/net/pdu/gudphdr.h \
	$${PWD}/../src/net/process/gclienthellosplit.h \
	$${PWD}/../src/net/process/gcorrectchecksum.h \
	$${PWD}/../src/net/process/gdnsprocessor.h \
	$${PWD}/../src/net/process/gflowmgrdebugger.h \
	$${PWD}/../src/net/process/gpacketdebugger.h \
	$${PWD}/../src/net/write/gpcapdevicewrite.h \
	$${PWD}/../src/net/write/gpcapfilewrite.h \
	$${PWD}/../src/net/write/gpcapwrite.h \
	$${PWD}/../src/net/write/grawipsocketwrite.h \
	$${PWD}/../src/net/write/gwrite.h

linux {
	SOURCES += \
		$${PWD}/../src/net/_linux/grtmlinux.cpp

	HEADERS += \
		$${PWD}/../src/net/_linux/grtmlinux.h
}
win32 {
	SOURCES += $${PWD}/../src/net/_win/gipadapterinfo.cpp
	SOURCES += $${PWD}/../src/net/_win/gipforwardtable.cpp
	SOURCES += $${PWD}/../src/net/_win/grtmwin32.cpp
	HEADERS += $${PWD}/../src/net/_win/gipadapterinfo.h
	HEADERS += $${PWD}/../src/net/_win/gipforwardtable.h
	HEADERS += $${PWD}/../src/net/_win/grtmwin32.h
	HEADERS += $${PWD}/../src/net/_win/windivert.h
}
