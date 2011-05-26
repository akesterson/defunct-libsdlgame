# This makefile is a bit hackish. I wrote it early in the AM. 
# Fohgiveuhness, please!!

ifndef $(CFG)
	CFG=Debug
endif

# you can also pass : mingw32 and macosx
ifndef $(OS)
	OS=linux
endif
 
TARGET=animate
BINTARGET=$(TARGET)
PROJECTHOME=$(shell pwd)
SDL_CFLAGS=$(shell sdl-config --cflags)
SDL_LDFLAGS=$(shell sdl-config --libs)
LIBDIR=/usr/lib
HEADERDIR=/usr/include
ADDL_CFLAGS=

ifeq "$(OS)" "mingw32"
	ADDL_CFLAGS=-mwindows -DBUILD_MINGW32
endif
ifeq "$(OS)" "linux"
	ADDL_CFLAGS=-DBUILD_LINUX
endif 

# default for release configs
ifeq "$(CFG)" "Release"
	OUTDIR=Release
	ifeq "$(OS)" "mingw32"
		BINTARGET=$(TARGET).exe
	else
		BINTARGET=$(TARGET)
	endif
	LINKLIB=game
	CXXFLAGS=-I../../../ -I$(HEADERDIR) -I./cpp -c $(SDL_CFLAGS) $(ADDL_CFLAGS)
endif
ifeq "$(CFG)" "Debug"
	OUTDIR=Debug
	ifeq "$(OS)" "mingw32"
		BINTARGET=$(TARGET)-dbg.exe
	else
		BINTARGET=$(TARGET)-dbg
	endif
	LINKLIB=game-dbg
	CXXFLAGS=-I../../../ -I$(HEADERDIR) -I./cpp  -pg -g -ggdb -gstabs -Wall -c $(SDL_CFLAGS) $(ADDL_CFLAGS)
endif

LINKLIBS=-L../../$(CFG) -L$(LIBDIR) -l$(LINKLIB) $(SDL_LDFLAGS) -lSDL_image -lSDL_mixer -lSDL_gfx -lSDL_ttf

BINOBJ=$(OUTDIR)/demo.o

CC = gcc
CXX = g++
LD = $(CXX)
INSTALL = $(which install)

$(OUTDIR)/%.o : cpp/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

all: bin

bin: $(BINOBJ)
	$(LD) -pg -o $(OUTDIR)/$(BINTARGET) \
		-pg $(BINOBJ) $(LINKLIBS)  

.PHONY: clean
clean:
	rm -f $(OUTDIR)/*.o
	rm -f $(OUTDIR)/$(BINTARGET)

.PHONY: rebuild
rebuild:
	make clean
	make CFG=$(CFG)
