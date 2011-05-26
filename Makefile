# This makefile is a bit hackish. I wrote it early in the AM. 
# Fohgiveuhness, please!!

ifndef $(CFG)
	CFG=Debug
endif
# you can also pass : mingw32 and macosx
ifndef $(OS)
	OS=linux
endif
 
LIBNAME=game
PROJECTHOME=$(shell pwd)
SDL_CFLAGS=$(shell sdl-config --cflags)
SDL_LDFLAGS=$(shell sdl-config --static-libs)
LIBDIR=/usr/lib
HEADERDIR=/usr/include
ADDL_CFLAGS=

ifeq "$(OS)" "mingw32"
	ADDL_CFLAGS=-mwindows
endif

ifeq "$(CFG)" "Debug"
	OUTDIR=Debug
	LIBTARGET=lib$(LIBNAME)-dbg
	LINKLIB=game-dbg
	CXXFLAGS=-pg -I./source -g -ggdb -gstabs -c $(SDL_CFLAGS) $(ADDL_CFLAGS)
endif

ifeq "$(CFG)" "Release"
	OUTDIR=Release
	LIBTARGET=lib$(LIBNAME)
	LINKLIB=game
	CXXFLAGS=-I./source -c $(SDL_CFLAGS) $(ADDL_CFLAGS)
endif

LINKLIBS=-L../../$(CFG) -L$(OUTDIR) -l$(LINKLIB) $(SDL_LDFLAGS) -lSDL_image -lSDL_mixer

LIBOBJ=$(OUTDIR)/Common.o \
	$(OUTDIR)/FontRenderer.o \
	$(OUTDIR)/Renderable.o \
	$(OUTDIR)/SpriteStrip.o \
	$(OUTDIR)/Animation.o \
	$(OUTDIR)/Actor.o \
	$(OUTDIR)/Display.o \
	$(OUTDIR)/Display2D.o \
	$(OUTDIR)/MenuDisplay.o \
	$(OUTDIR)/Game.o

CC = gcc
CXX = g++
LD = $(CXX)
INSTALL = $(shell which install)

$(OUTDIR)/%.o : %.cpp
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
	doxygen doxygen.conf

.PHONY: clean
clean:
	rm -f $(OUTDIR)/*.o
	rm -f $(OUTDIR)/$(LIBTARGET).*

.PHONY: demos
demos:
	cd demo && ./build-demos.sh $(CFG)

.PHONY: rebuild
rebuild:
	make clean
	make CFG=$(CFG) OS=$(OS)


.PHONY: install
install:
	$(INSTALL) $(OUTDIR)/$(LIBTARGET)* $(LIBDIR)/
	mkdir -p $(HEADERDIR)/libgame
	$(INSTALL) *h $(HEADERDIR)/libgame/

.PHONY: deps
deps:
	cd deps && make