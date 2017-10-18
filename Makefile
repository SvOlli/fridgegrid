
QMAKE ?= qmake

all: release

debug release: src/Makefile
	make -C src $@

src/Makefile:
	(cd src;$(QMAKE) -r CONFIG+=debug_and_release)

clean:
	[ ! -f src/Makefile ] || make -C src distclean
	rm -rf build

exe:
	(cd packaging/windows ; ./build-cross.sh)

dmg:
	(cd packaging/macosx ; ./build-app.sh)

