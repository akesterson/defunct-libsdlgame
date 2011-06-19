# This makefile is a bit hackish. I wrote it early in the AM. 
# Fohgiveuhness, please!!


ifndef $(CFG)
	CFG=Debug
endif

# you can also pass : mingw32 and macosx
ifndef $(OS)
	OS=linux
endif
 
TARGET=demo
BINTARGET=$(TARGET)
PROJECTHOME=$(shell pwd)
SDL_CFLAGS=$(shell sdl-config --cflags)
SDL_LDFLAGS=$(shell sdl-config --libs)
LIBDIR=/usr/lib
HEADERDIR=/usr/include
ADDL_CFLAGS=

ifeq "$(OS)" "mingw32"
	ADDL_CFLAGS=-mwindows -DBUILD_MINGW32 -rdynamic
endif
ifeq "$(OS)" "linux"
	ADDL_CFLAGS=-DBUILD_LINUX -rdynamic
endif 

# default for release configs
ifeq "$(CFG)" "Release"
	OUTDIR=Release
	ifeq "$(OS)" "mingw32"
		BINTARGET=$(TARGET).exe
	else
		BINTARGET=$(TARGET)
	endif
	LINKLIB=sdlgame
	CXXFLAGS=-I../../ -I$(HEADERDIR) -I./cpp -c $(SDL_CFLAGS) $(ADDL_CFLAGS)
endif
ifeq "$(CFG)" "Debug"
	OUTDIR=Debug
	ifeq "$(OS)" "mingw32"
		BINTARGET=$(TARGET)-dbg.exe
	else
		BINTARGET=$(TARGET)-dbg
	endif
	LINKLIB=sdlgame-dbg
	CXXFLAGS=-I../../ -I$(HEADERDIR) -I./cpp  -pg -g -ggdb -gstabs -Wall -c $(SDL_CFLAGS) $(ADDL_CFLAGS)
endif

LINKLIBS=-L../../$(CFG) -L$(LIBDIR) -l$(LINKLIB) $(SDL_LDFLAGS) -lSDL_image -lSDL_mixer -lSDL_gfx -lSDL_ttf

BINOBJ=$(OUTDIR)/demo.o

ifeq "$(OS)" "mingw32"
	BTOBJ=$(OUTDIR)/backtrace.dll
endif

CC = gcc
CXX = g++
LD = $(CXX)
INSTALL = $(which install)

$(OUTDIR)/%.o : cpp/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

all: bin

.PHONY: $(BTOBJ)
$(BTOBJ): 
	cp /usr/local/lib/backtrace.dll $(OUTDIR)/
	cp /usr/local/lib/av*dll $(OUTDIR)/
	cp /usr/local/lib/swscale*dll $(OUTDIR)/
	cp /usr/local/bin/*SDL*dll $(OUTDIR)/
	cp /usr/local/bin/freetype*dll $(OUTDIR)/
	cp /usr/local/bin/*png*dll $(OUTDIR)/
	cp /usr/local/bin/*jpeg*dll $(OUTDIR)/
	cp /usr/local/bin/libimage*dll $(OUTDIR)/
	cp /usr/local/bin/libogg*dll $(OUTDIR)/
	cp /usr/local/bin/*vorbis*dll $(OUTDIR)/
	cp /usr/local/bin/librle*dll $(OUTDIR)/
	cp /usr/local/bin/libtiff*dll $(OUTDIR)/
	cp /usr/local/bin/libxml2*dll $(OUTDIR)/
	cp /usr/local/bin/zlib*dll $(OUTDIR)/

bin: $(BINOBJ) $(BTOBJ)
	$(LD) -pg -o $(OUTDIR)/$(BINTARGET) \
		-pg $(BINOBJ) $(LINKLIBS)  

.PHONY: clean
clean:
	rm -f $(OUTDIR)/*

.PHONY: rebuild
rebuild:
	make clean
	make CFG=$(CFG)
