# This makefile is a bit hackish. I wrote it early in the AM. 
# Fohgiveuhness, please!!

ifndef $(CFG)
	CFG=Debug
endif
# you can also pass : linux and macosx
ifndef $(OS)
	OS=mingw32
endif
 
LIBNAME=sdlgame
PROJECTHOME=$(shell pwd)
SDL_CFLAGS=$(shell sdl-config --cflags)
SDL_LDFLAGS=$(shell sdl-config --static-libs)
LIBDIR=/usr/lib
HEADERDIR=/usr/include
ADDL_CFLAGS=
OBJDIR=libsdlgame
EXESUFFIX=
LIBSUFFIX=

ifeq "$(OS)" "mingw32"
	ADDL_CFLAGS=-rdynamic -mwindows -DOS_WIN32
	EXESUFFIX=.exe
	LIBSUFFIX=.a
	DEPMAKEFILE=Makefile.mingw32
endif

ifeq "$(OS)" "macosx"
	ADDL_CFLAGS=-rdynamic -DOS_MACOSX
	DEPMAKEFILE=Makefile.macosx
endif

ifeq "$(OS)" "linux"
	ADDL_CFLAGS=-rdynamic -DOS_LINUX
	DEPMAKEFILE=Makefile.apt
	LIBSUFFIX=.a
endif

ifeq "$(CFG)" "Debug"
	OUTDIR=Debug
	LIBTARGET=lib$(LIBNAME)-dbg$(LIBSUFFIX)
	LINKLIB=game-dbg
	CXXFLAGS=-pg -I./source -g -ggdb -gstabs -c $(SDL_CFLAGS) $(ADDL_CFLAGS)
endif

ifeq "$(CFG)" "Release"
	OUTDIR=Release
	LIBTARGET=lib$(LIBNAME)$(LIBSUFFIX)
	LINKLIB=game
	CXXFLAGS=-I./source -c $(SDL_CFLAGS) $(ADDL_CFLAGS)
endif

LINKLIBS=-L../../$(CFG) -L$(OUTDIR) -l$(LINKLIB) $(SDL_LDFLAGS) -lSDL_image -lSDL_mixer

LIBOBJ=$(OBJDIR)/Backtrace.o \
	$(OBJDIR)/Common.o \
	$(OBJDIR)/FontRenderer.o \
	$(OBJDIR)/Renderable.o \
	$(OBJDIR)/SpriteStrip.o \
	$(OBJDIR)/Animation.o \
	$(OBJDIR)/Actor.o \
	$(OBJDIR)/Display.o \
	$(OBJDIR)/Display2D.o \
	$(OBJDIR)/MenuDisplay.o \
	$(OBJDIR)/Game.o

DEMOS=backtrace \
	bouncingball \
	exploder \
	explodingball \
	frictionball \
	helloworld \
	gravity \
	tilemap \

CC = gcc
CXX = g++
LD = $(CXX)
INSTALL = $(shell which install)

$(OUTDIR)/%.o : $(OBJDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<
ifeq "$(OS)" "macosx"
all: sharedlib
else
all: staticlib
endif

# this was a pain in my ass cross-platform, so I killed it
sharedlib: $(LIBOBJ)
	$(CXX) -dynamiclib -Wl,-headerpad_max_install_names,-undefined,dynamic_lookup,-compatibility_version,1.0,-current_version,1.0,-install_name,/usr/lib/$(LIBTARGET).dylib -o $(CFG)/$(LIBTARGET).dylib $(LIBOBJ)

staticlib: $(LIBOBJ)
	$(AR) rcs $(OUTDIR)/$(LIBTARGET) $(LIBOBJ)

.PHONY: docs
docs:
	cd libsdlgame && doxygen doxygen.conf

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(OUTDIR)/$(LIBTARGET).*
	rm -rf docs/*
	cd demo && for dir in $(DEMOS); do cd $$dir && make CFG=$(CFG) OS=$(OS) clean; if [ $$? -ne 0 ]; then exit 1 ; fi; cd .. ; done

.PHONY: demos
demos:
	cd demo && for dir in $(DEMOS); do cd $$dir && make "ADDL_CFLAGS=$(ADDL_CFLAGS)" CFG=$(CFG) OS=$(OS); if [ $$? -ne 0 ]; then exit 1 ; fi; cd .. ; done

.PHONY: rebuild
rebuild:
	make clean
	make CFG=$(CFG) OS=$(OS)


.PHONY: install
install:
	$(INSTALL) $(OUTDIR)/$(LIBTARGET)* $(LIBDIR)/
	mkdir -p $(HEADERDIR)/libsdlgame
	$(INSTALL) $(OBJDIR)/*h $(HEADERDIR)/libsdlgame/

.PHONY: uninstall
uninstall:
	rm $(LIBDIR)/$(LIBTARGET)*
	rm -rf $(HEADERDIR)/libsdlgame

.PHONY: deps
deps:
	cd deps && make -f $(DEPMAKEFILE)
