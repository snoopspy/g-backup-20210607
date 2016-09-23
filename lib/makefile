.PHONY: all lib test debug release debug_test release_test libg_d libg_gui_d libg libg_gui g_d_test g_gui_d_test g_test g_gui_test

all: lib #test

lib: release #debug

test: debug_test release_test

debug: libg_d libg_gui_d

release: libg libg_gui

debug_test: g_d_test g_gui_d_test

release_test: g_test g_gui_test

libg_d:
	mkdir -p libg_d;      cd libg_d;        qmake ../libg.pro          "CONFIG+=debug"; make -j 4
	
libg_gui_d:
	mkdir -p libg_gui_d;   cd libg_gui_d;   qmake ../libg_gui.pro      "CONFIG+=debug"; make -j 4

libg:
	mkdir -p libg;         cd libg;         qmake ../libg.pro;                          make -j 4

libg_gui:
	mkdir -p libg_gui;     cd libg_gui;     qmake ../libg_gui.pro;                      make -j 4

g_d_test:
	mkdir -p g_d_test;     cd g_d_test;     qmake ../libg_test.pro     "CONFIG+=debug"; make -j 4

g_gui_d_test:
	mkdir -p g_gui_d_test; cd g_gui_d_test; qmake ../libg_gui_test.pro "CONFIG+=debug"; make -j 4

g_test:
	mkdir -p g_test;       cd g_test;       qmake ../libg_test.pro;                     make -j 4

g_gui_test:
	mkdir -p g_gui_test;   cd g_gui_test;   qmake ../libg_gui_test.pro;                 make -j 4

clean:
	rm -rf libg_d
	rm -rf libg_gui_d
	rm -rf libg
	rm -rf libg_gui
	rm -rf g_d_test
	rm -rf g_gui_d_test
	rm -rf g_test
	rm -rf g_gui_test
	rm -rf *.pro.user

distclean: clean
	find ../bin -name "*.so*" -delete
	find ../bin -type f -executable -name "*_test" -delete
	rm -f ../bin/g_d_test
	rm -f ../bin/g_gui_d_test
	rm -f ../bin/g_test
	rm -f ../bin/g_gui_test
