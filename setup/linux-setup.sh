export QTDIR=/opt/Qt/5.15.2/gcc_64

#
# directory
#
rm -rf linux-setup
mkdir linux-setup
mkdir linux-setup/platforms

#
# qt files
#
cp $QTDIR/lib/libQt5Core.so.5 linux-setup
cp $QTDIR/lib/libQt5DBus.so.5 linux-setup
cp $QTDIR/lib/libQt5Gui.so.5 linux-setup
cp $QTDIR/lib/libQt5Network.so.5 linux-setup
cp $QTDIR/lib/libQt5Widgets.so.5 linux-setup
cp $QTDIR/lib/libQt5XcbQpa.so.5 linux-setup

cp $QTDIR/lib/libicudata.so.56 linux-setup
cp $QTDIR/lib/libicui18n.so.56 linux-setup
cp $QTDIR/lib/libicuuc.so.56 linux-setup

#
# platforms files
#
cp $QTDIR/plugins/platforms/* linux-setup/platforms

#
# g files 
#
cp ../bin/netclient linux-setup; strip linux-setup/netclient
cp ../bin/netserver linux-setup; strip linux-setup/netserver
cp ../bin/sscon linux-setup; strip linux-setup/sscon
cp ../bin/snoopspy linux-setup; strip linux-setup/snoopspy
cp ../bin/sslog linux-setup; strip linux-setup/sslog
mkdir linux-setup/ss
cp ../bin/ss/* linux-setup/ss

#
# compress
#
cd linux-setup
tar czf snoopspy-$(sed 's/"//g' ../../version.txt).tar.gz *
#tar czf snoopspy-$(date '+%Y%m%d').tar.gz *
cd ..

