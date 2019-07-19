rem for win64
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64
set path=%path%;C:\Qt\5.12.2\msvc2017_64\bin
rem for win32
rem call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x86
rem set path=%path%;C:\Qt\5.12.2\msvc2017\bin

rem ***************************************************************************
rem lib
rem ***************************************************************************
cd lib
	mkdir libg
	cd libg
	qmake ../libg.pro QMAKE_CXXFLAGS+=/MP
	nmake
	cd ..
	mkdir libg_gui
	cd libg_gui
	qmake ../libg_gui.pro QMAKE_CXXFLAGS+=/MP
	nmake
	cd ..
cd ..

rem ***************************************************************************
rem plugin
rem ***************************************************************************
cd plugin
qmake g_plugin.pro QMAKE_CXXFLAGS+=/MP
nmake
cd ..

rem ***************************************************************************
rem app
rem ***************************************************************************
cd app
qmake g_app.pro QMAKE_CXXFLAGS+=/MP
nmake
cd ..
