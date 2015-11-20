export QTDIR=/opt/qt/5.5/gcc_64

#
# directory
#
rm -rf setup
mkdir setup
mkdir setup/platforms

#
# qt files
#
cp $QTDIR/lib/libQt5Core.so.5 setup
cp $QTDIR/lib/libQt5DBus.so.5 setup
cp $QTDIR/lib/libQt5Gui.so.5 setup
cp $QTDIR/lib/libQt5Network.so.5 setup
cp $QTDIR/lib/libQt5Widgets.so.5 setup
cp $QTDIR/lib/libQt5XcbQpa.so.5 setup

cp $QTDIR/lib/libicudata.so.54 setup
cp $QTDIR/lib/libicui18n.so.54 setup
cp $QTDIR/lib/libicuuc.so.54 setup

#
# platforms files
#
cp $QTDIR/plugins/platforms/* setup/platforms

#
# g files 
#
cp netclient    setup
cp netclient.sh setup

#
# compress
#
cd setup
tar czf ../gbin.tar.gz *
cd ..

