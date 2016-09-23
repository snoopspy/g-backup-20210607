.PHONY: all lib app plugin clean distclean

all: lib app plugin

lib:
	cd lib && make && cd ..

app:
	cd app && qmake && make -j 4 && cd ..

plugin:
	cd plugin && qmake && make -j 4 && cd ..

clean:
	cd lib && make clean; true
	cd app && make clean; true
	cd plugin && make clean; true
	find -type d -name 'build-*'    -exec rm -r {} \; | true
	find -type f -name '*.o'        -delete
	find -type f -name '*.pro.user' -delete

distclean: clean
	cd lib && make distclean; true
	cd app && make distclean; true
	find bin -type f -executable -delete
	find bin -type f -name "*.json" -delete
	rm -rf setup/setup
	rm -rf setup/*.gz
	find -type f -name 'Makefile*'  -delete
