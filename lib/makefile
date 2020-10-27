.PHONY: all lib test debug release debug-test release-test libg-d libg-gui-d libg libg-gui g-d-test g-gui-d-test g-test g-gui-test

all: lib #test

lib: release #debug

test: debug-test release-test

debug: libg-d libg-gui-d

release: libg libg-gui

debug-test: g-d-test g-gui-d-test

release-test: g-test g-gui-test

libg-d:
	mkdir -p libg-d;      cd libg-d;        qmake ../libg.pro          "CONFIG+=debug";   make -j4
	
libg-gui-d:
	mkdir -p libg-gui-d;   cd libg-gui-d;   qmake ../libg-gui.pro      "CONFIG+=debug";   make -j4

libg:
	mkdir -p libg;         cd libg;         qmake ../libg.pro          "CONFIG+=release"; make -j4

libg-gui:
	mkdir -p libg-gui;     cd libg-gui;     qmake ../libg-gui.pro      "CONFIG+=release"; make -j4

g-d-test:
	mkdir -p g-d-test;     cd g-d-test;     qmake ../libg-test.pro     "CONFIG+=debug";   make -j4

g-gui-d-test:
	mkdir -p g-gui-d-test; cd g-gui-d-test; qmake ../libg-gui-test.pro "CONFIG+=debug";   make -j4

g-test:
	mkdir -p g-test;       cd g-test;       qmake ../libg-test.pro     "CONFIG+=release"; make -j4

g-gui-test:
	mkdir -p g-gui-test;   cd g-gui-test;   qmake ../libg-gui-test.pro "CONFIG+=release"; make -j4

clean:
	rm -rf libg-d
	rm -rf libg-gui-d
	rm -rf libg
	rm -rf libg-gui
	rm -rf g-d-test
	rm -rf g-gui-d-test
	rm -rf g-test
	rm -rf g-gui-test
	rm -rf *.pro.user

distclean: clean
	find ../bin -name "*.so*" -delete
	find ../bin -type f -executable -name "*-test" -delete
	rm -f ../bin/g-d-test
	rm -f ../bin/g-gui-d-test
	rm -f ../bin/g-test
	rm -f ../bin/g-gui-test
