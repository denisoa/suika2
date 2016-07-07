include ../common.mk

#
# Toolchain selection
#

CC = gcc

#
# CPPFLAGS
#

CPPFLAGS = \
	-I./libroot/include \
	-I./libroot/include/freetype2

#
# CFLAGS
#

CFLAGS = \
	-arch i386 \
	-arch x86_64 \
	-O0 \
	-g3 \
	-ftree-vectorize \
	-std=gnu89 \
	-Wall \
	-Werror \
	-Wextra \
	-Wundef \
	-Wconversion

#
# LDFLAGS
#

LDFLAGS = \
	-arch i386 \
	-arch x86_64 \
	-framework Cocoa \
	-framework AudioUnit \
	-lpthread \
	-L./libroot/lib \
	-lz \
	-lpng16 \
	-logg \
	-lvorbis \
	-lvorbisfile \
	-lfreetype

#
# Source files
#

SRCS_C = \
	$(SRCS_COMMON) \
	$(SRCS_SSE) \
	../../src/aunit.c


SRCS_M = \
	../../src/nsmain.m

#
# .c/.m Compilation rules
#

OBJS = \
	$(SRCS_C:../../src/%.c=%.o) \
	$(SRCS_M:../../src/%.m=%.o)

%.o: ../../src/%.m
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

%.o: ../../src/%.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

#
# Target
#

Suika.app/Contents/MacOS/Suika: $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OBJS) $(LDFLAGS)

libroot/include/zlib.h:
	./build-libs.sh

#
# Feature specific compilation rules.
#

include ../sse.mk

#
# Phony rules
#

run: $(TARGET)
	cp -R Suika.app ../../suika2/ && ../../suika2/Suika.app/Contents/MacOS/Suika

clean:
	rm -f $(TARGET) $(OBJS) *~

erase:
	rm -rf libroot
