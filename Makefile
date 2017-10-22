
QMAKE ?= qmake
BUILDDIR = build

all: release

.PHONY: debug release exe windows dmg macosx

release: $(BUILDDIR)-release/Makefile
	+make -C $(BUILDDIR)-release all

debug: $(BUILDDIR)-debug/Makefile
	+make -C $(BUILDDIR)-debug all

$(BUILDDIR)-release/Makefile: $(BUILDDIR)-release
	(cd $(BUILDDIR)-release;$(QMAKE) -r CONFIG+=release CONFIG-=debug CONFIG-=debug_and_release ../src/src.pro)

$(BUILDDIR)-debug/Makefile: $(BUILDDIR)-debug
	(cd $(BUILDDIR)-debug;$(QMAKE) -r CONFIG+=debug CONFIG-=release CONFIG-=debug_and_release ../src/src.pro)

$(BUILDDIR)-release $(BUILDDIR)-debug:
	mkdir -p $@

distclean clean:
	rm -rf $(BUILDDIR)-release $(BUILDDIR)-debug

exe windows:
	packaging/windows/build-cross.sh

dmg macosx:
	packaging/macosx/build-app.sh

