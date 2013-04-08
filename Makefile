
QMAKE ?= qmake

all: release

debug release: src/Makefile
	+(cd src;make $(MAKEFLAGS) $@)

src/Makefile:
	(cd src;$(QMAKE) -r CONFIG+=debug_and_release)

clean:
	(cd src;make distclean)
	rm -rf build

exe:
	(cd packaging/windows ; ./build-cross.sh)

dmg:
	(cd packaging/macosx ; ./build-app.sh)

