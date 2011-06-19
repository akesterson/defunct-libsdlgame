all: clean packages

CFLAGS=$(CFLAGS) -I/usr/local/include -I/usr/include
LDFLAGS=$(LDFLAGS) -L/usr/local/lib -L/usr/lib

.PHONY: packages
packages: SDL zlib libpng libjpeg libtiff SDL_image libvorbis flac SDL_mixer freetype SDL_ttf SDL_gfx libxml2 cmake ffmpeg SDL_ffmpeg backtrace

.PHONY: SDL
SDL:
	wget http://www.libsdl.org/release/SDL-1.2.14.tar.gz
	tar -zxvf SDL-1.2.14.tar.gz
	cd SDL-1.2.14 && ./configure  && make && make install

.PHONY: libpng
libpng:
	wget -O libpng-1.2.37-bin.zip http://downloads.sourceforge.net/gnuwin32/libpng-1.2.37-bin.zip
	unzip -od /usr/local/ libpng-1.2.37-bin.zip
	wget -O libpng-1.2.37-deps.zip http://downloads.sourceforge.net/gnuwin32/libpng-1.2.37-dep.zip
	unzip -od /usr/local/ libpng-1.2.37-deps.zip
	wget -O libpng-1.2.37-devs.zip http://downloads.sourceforge.net/gnuwin32/libpng-1.2.37-lib.zip
	unzip -od /usr/local/ libpng-1.2.37-devs.zip
	cp libpng-config /usr/local/bin/libpng-config

.PHONY: zlib
zlib:
	wget -O zlib-bin.zip http://gnuwin32.sourceforge.net/downlinks/zlib-bin-zip.php
	unzip -od /usr/local/ zlib-bin.zip
	wget -O zlib-libs.zip http://gnuwin32.sourceforge.net/downlinks/zlib-lib-zip.php
	unzip -od /usr/local/ zlib-libs.zip

.PHONY: libjpeg
libjpeg:
	wget -O jpeg-bin.zip http://gnuwin32.sourceforge.net/downlinks/jpeg-bin-zip.php
	unzip -od /usr/local/ jpeg-bin.zip
	wget -O jpeg-deps.zip http://gnuwin32.sourceforge.net/downlinks/jpeg-dep-zip.php
	unzip -od /usr/local/ jpeg-deps.zip
	wget -O jpeg-devs.zip http://gnuwin32.sourceforge.net/downlinks/jpeg-lib-zip.php
	unzip -od /usr/local/ jpeg-devs.zip

.PHONY: libtiff
libtiff:
	wget -O tiff-bin.zip http://gnuwin32.sourceforge.net/downlinks/tiff-bin-zip.php
	unzip -od /usr/local/ tiff-bin.zip
	wget -O tiff-deps.zip http://gnuwin32.sourceforge.net/downlinks/tiff-dep-zip.php
	unzip -od /usr/local/ tiff-deps.zip
	wget -O tiff-devs.zip http://gnuwin32.sourceforge.net/downlinks/tiff-lib-zip.php
	unzip -od /usr/local/ tiff-devs.zip


.PHONY: SDL_image
SDL_image:
	wget http://www.libsdl.org/projects/SDL_image/release/SDL_image-1.2.10.tar.gz
	tar -zxvf SDL_image-1.2.10.tar.gz
	cd SDL_image-1.2.10 && CFLAGS="-I/usr/local/include -I/usr/include" LDFLAGS=-L/usr/local/lib ./configure && make && make install

.PHONY: libvorbis
libvorbis:
	wget http://downloads.xiph.org/releases/ogg/libogg-1.2.2.tar.gz
	tar -zxvf libogg-1.2.2.tar.gz
	cd libogg-1.2.2 && ./configure && make && make install
	wget http://downloads.xiph.org/releases/vorbis/libvorbis-1.3.2.tar.gz
	tar -zxvf libvorbis-1.3.2.tar.gz
	cd libvorbis-1.3.2 && CFLAGS=-I/usr/local/include LDFLAGS=-L/usr/local/lib ./configure && make && make install

.PHONY: flac
flac:
	wget http://downloads.xiph.org/releases/flac/flac-1.2.1.tar.gz
	tar -zxvf flac-1.2.1.tar.gz
	patch -p0 < flac-alloc.h.patch
	patch -p0 < flac-main.cpp.patch
	cd flac-1.2.1 && ./configure && make && make install

.PHONY: SDL_mixer
SDL_mixer:
	wget http://www.libsdl.org/projects/SDL_mixer/release/SDL_mixer-1.2.11.tar.gz
	tar -zxvf SDL_mixer-1.2.11.tar.gz
	cd SDL_mixer-1.2.11 && CFLAGS=-I/usr/local/include LDFLAGS=-L/usr/local/lib ./configure && make && make install

.PHONY: SDL_ttf
SDL_ttf:
	wget http://www.libsdl.org/projects/SDL_ttf/release/SDL_ttf-2.0.10.tar.gz
	tar -zxvf SDL_ttf-2.0.10.tar.gz
	cd SDL_ttf-2.0.10 && CFLAGS="-I/usr/local/include -I/usr/local/include/freetype2" LDFLAGS=-L/usr/local/lib ./configure && make && make install

.PHONY: freetype
freetype:
	wget -O freetype-bin.zip http://gnuwin32.sourceforge.net/downlinks/freetype-bin-zip.php
	unzip -od /usr/local/ freetype-bin.zip
	wget -O freetype-deps.zip http://gnuwin32.sourceforge.net/downlinks/freetype-dep-zip.php
	unzip -od /usr/local/ freetype-deps.zip
	wget -O freetype-devs.zip http://gnuwin32.sourceforge.net/downlinks/freetype-lib-zip.php
	unzip -od /usr/local/ freetype-devs.zip

.PHONY: SDL_gfx
SDL_gfx:
	wget http://www.ferzkopp.net/Software/SDL_gfx-2.0/SDL_gfx-2.0.22.tar.gz
	tar -zxvf SDL_gfx-2.0.22.tar.gz
	mkdir SDL_gfx-2.0.22/m4
	cp /usr/local/share/aclocal/sdl.m4 SDL_gfx-2.0.22/m4
	cd SDL_gfx-2.0.22 && libtoolize --force --copy && aclocal -I m4 && autoconf && automake
	cd SDL_gfx-2.0.22 && CFLAGS="-I/usr/local/include -I/usr/include" LDFLAGS="-L/usr/local/lib -L/usr/lib" ./configure && make && make install

.PHONY: libxml2
libxml2:
	wget http://sourceforge.net/projects/gnuwin32/files/libxml/2.4.12-1/libxml2-2.4.12-bin.zip/download
	unzip -od /usr/local/ libxml2-2.4.12-bin.zip
	wget http://sourceforge.net/projects/gnuwin32/files/libxml/2.4.12-1/libxml2-2.4.12-1-lib.zip/download
	unzip -od /usr/local/ libxml2-2.4.12-1-lib.zip

.PHONY: doxygen
doxygen:
	wget http://ftp.stack.nl/pub/users/dimitri/doxygen-1.7.4.windows.bin.zip
	unzip -od /bin doxygen-1.7.4.windows.bin.zip

# Just for the record - cmake is fuckin bullshit.
.PHONY: cmake
cmake:
	wget http://www.cmake.org/files/v2.8/cmake-2.8.4-win32-x86.zip
	unzip cmake-2.8.4-win32-x86.zip
	cp -vR cmake-2.8.4-win32-x86/* /usr/local/
#wget http://www.cmake.org/files/v2.8/cmake-2.8.4.tar.gz
#cd cmake-2.8.4 && ./configure && make && make install

.PHONY: ffmpeg
ffmpeg:
	wget http://www.ffmpeg.org/releases/ffmpeg-0.7-rc1.tar.gz
	tar -zxvf ffmpeg-0.6.3.tar.gz
	cd ffmpeg-0.6.3 && ./configure $(FFMPEG_CONFIGURE_FLAGS) && make && make install

.PHONY: SDL_ffmpeg
SDL_ffmpeg:
	wget http://www.arjanhouben.nl/SDL_ffmpeg/1.3.1/SDL_ffmpeg-1.3.1.tar.gz
	tar -zxvf SDL_ffmpeg-1.3.1.tar.gz
	cd SDL_ffmpeg && cmake -G "MSYS Makefiles" . && make
	cp SDL_ffmpeg/lib/libSDL_ffmpeg.dll.a /usr/local/lib/
	cp SDL_ffmpeg/include/SDL/SDL_ffmpeg.h /usr/local/include

.PHONY: yaml-cpp
yaml-cpp:
	wget http://yaml-cpp.googlecode.com/files/yaml-cpp-0.2.6.tar.gz
	tar -zxvf yaml-cpp-0.2.6.tar.gz
	cd yaml-cpp && cmake -G "MSYS Makefiles" . && make
	cp -vR yaml-cpp/include/yaml-cpp /usr/local/include/
	cp yaml-cpp/libyaml-cpp.a /usr/local/lib/

.PHONY: backtrace
backtrace:
	svn checkout http://backtrace-mingw.googlecode.com/svn/trunk/ backtrace-mingw-read-only
	cd backtrace-mingw-read-only && make && cp backtrace.dll /usr/local/lib/

.PHONY: clean
clean:	
	rm -rf *gz *zip *bz2
	find . -maxdepth 1 -type d -iname "[a-zA-Z0-9]*" -exec rm -rf \{\} \;