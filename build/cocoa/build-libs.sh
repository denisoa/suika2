#!/bin/sh

PREFIX=`pwd`/libroot

rm -rf tmp libroot
mkdir -p tmp libroot

cd tmp

tar xzf ../../libsrc/zlib-1.2.11.tar.gz
cd zlib-1.2.11
CFLAGS="-mmacosx-version-min=10.6" ./configure --prefix=$PREFIX --static --archs="-arch i386 -arch x86_64"
make
make install
cd ..

tar xzf ../../libsrc/libpng-1.6.28.tar.gz
cd libpng-1.6.28
./configure --prefix=$PREFIX --disable-shared CPPFLAGS=-I$PREFIX/include CFLAGS="-arch i386 -arch x86_64 -mmacosx-version-min=10.6" LDFLAGS="-L$PREFIX/lib -arch i386 -arch x86_64 -mmacosx-version-min=10.6"
make
make install
cd ..

tar xzf ../../libsrc/libogg-1.3.2.tar.gz
cd libogg-1.3.2
./configure --prefix=$PREFIX --disable-shared CFLAGS="-arch i386 -arch x86_64 -mmacosx-version-min=10.6" LDFLAGS="-arch i386 -arch x86_64 -mmacosx-version-min=10.6"
make
make install
cd ..


tar xzf ../../libsrc/libvorbis-1.3.5.tar.gz
cd libvorbis-1.3.5
./configure --prefix=$PREFIX --disable-shared --with-ogg-includes=$PREFIX/include --with-ogg-libraries=$PREFIX/lib CFLAGS="-arch i386 -arch x86_64 -mmacosx-version-min=10.6" LDFLAGS="-arch i386 -arch x86_64 -mmacosx-version-min=10.6"
make
make install
cd ..

tar xzf ../../libsrc/freetype-2.7.1.tar.gz
cd freetype-2.7.1
./configure --prefix=$PREFIX --disable-shared --with-bzip2 --with-png=no --with-zlib=no --with-harfbuzz=no --with-bzip2=no CFLAGS="-arch i386 -arch x86_64 -mmacosx-version-min=10.6" LDFLAGS="-arch i386 -arch x86_64 -mmacosx-version-min=10.6"
make
make install
cd ..

cd ..
rm -rf tmp
