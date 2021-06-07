#
# linux
#
make
make distclean

#
# android
#
source ../android-build/android.profile 
./android-build.sh
make distclean
