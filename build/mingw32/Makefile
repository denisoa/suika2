include ../common.mk

#
# Toolchain
#

#CC = gcc
#WINDRES = windres

# For cross compilation
CC = i686-w64-mingw32-gcc
WINDRES = i686-w64-mingw32-windres

#
# CPPFLAGS
#

CPPFLAGS = \
	-I. \
	-Ires \
	-I./libroot/include \
	-I./libroot/include/freetype2

#
# CFLAGS
#

CFLAGS = \
	-O3 \
	-fopt-info-vec \
	-std=gnu89 \
	-finput-charset=utf-8 \
	-fexec-charset=cp932 \
	-Wall \
	-Werror \
	-Wextra \
	-Wundef \
	-Wconversion

#
# LDFLAGS
#

LDFLAGS = \
	-mwindows \
	-lgdi32 \
	-lole32 \
	-ldxguid \
	-ldsound \
	-L./libroot/lib \
	-Wl,-dn,-lpng16,-lz,-lvorbisfile,-lvorbis,-logg,-lfreetype,-dy

#
# Source files
#

SRCS_C = \
	$(SRCS_COMMON) \
	$(SRCS_SSE) \
	../../src/winmain.c \
	../../src/dsound.c

SRCS_R = res/resource.rc

#
# .c.o compilation rules
#

OBJS = \
	$(SRCS_C:../../src/%.c=%.o) \
	$(SRCS_R:res/%.rc=%.o)

%.o: ../../src/%.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

%.o: res/%.rc
	$(WINDRES) $< $@

#
# Target
#

all: suika.exe

suika.exe: $(OBJS)
	$(CC) -o suika.exe $(OBJS) $(LDFLAGS)

#
# Feature specific source files.
#

novec.o: ../../src/novec.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -fno-tree-vectorize $<

sse.o: ../../src/sse.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -ftree-vectorize -msse $<

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
# Phony rules
#

run: suika.exe
	cp suika.exe ../../suika2; cd ../../suika2; ./suika.exe

clean:
	rm -f *.o suika.exe *~

erase:
	rm -rf libroot
