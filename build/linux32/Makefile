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
	-O3 \
	-fopt-info-vec \
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
	-lm \
	-lpthread \
	-lasound \
	-lX11 \
	-lXpm \
	-L./libroot/lib \
	-Wl,-dn,-lpng16,-lz,-lvorbisfile,-lvorbis,-logg,-lfreetype,-dy

#
# Source files
#

SRCS = \
	../../src/cpuid.c \
	../../src/novec.c \
	../../src/sse.c \
	../../src/sse2.c \
	../../src/sse3.c \
	../../src/sse41.c \
	../../src/sse42.c \
	../../src/avx.c \
	../../src/avx2.c \
	../../src/avx512.c \
	../../src/x11main.c \
	../../src/asound.c \
	../../src/image.c \
	../../src/readimage.c \
	../../src/glyph.c \
	../../src/wave.c \
	../../src/script.c \
	../../src/log.c \
	../../src/stage.c \
	../../src/suika.c \
	../../src/cmd_bg.c \
	../../src/cmd_bgm.c \
	../../src/cmd_ch.c \
	../../src/cmd_click.c

#
# .c.o compilation rules
#

OBJS = $(SRCS:../../src/%.c=%.o) \

%.o: ../../src/%.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

#
# Target
#

suika: $(OBJS) libroot/include/zlib.h
	$(CC) -o suika $(OBJS) $(LDFLAGS)

libroot/include/zlib.h:
	./build-libs.sh

#
# Feature specific source files.
#

novec.o: ../../src/novec.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -fno-tree-vectorize $<

sse2.o: ../../src/sse2.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -ftree-vectorize -msse2 $<

sse3.o: ../../src/sse3.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -ftree-vectorize -msse3 $<

sse41.o: ../../src/sse41.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -ftree-vectorize -msse4.1 $<

sse42.o: ../../src/sse42.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -ftree-vectorize -msse4.2 $<

avx.o: ../../src/avx.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -ftree-vectorize -mavx $<

avx2.o: ../../src/avx2.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -ftree-vectorize -mavx2 $<

avx512.o: ../../src/avx512.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -ftree-vectorize -mavx512f $<

#
# Phony
#

run: suika
	cp suika ../../suika2; cd ../../suika2; ./suika

clean:
	rm -rf *~ *.o suika

erase:
	rm -rf libroot
