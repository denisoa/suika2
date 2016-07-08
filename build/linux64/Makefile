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
	-ftree-vectorize \
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

include ../common.mk

SRCS = \
$(SRCS_COMMON) \
$(SRCS_SSE) \
../../src/asound.c \
../../src/x11main.c

#
# .c.o compilation rules
#

OBJS = $(SRCS:../../src/%.c=%.o) \

%.o: ../../src/%.c
	$(CC) -c $(CPPFLAGS) -O0 $(CFLAGS) $<

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

include ../sse.mk

#
# Phony
#

run: suika
	cp suika ../../suika2; cd ../../suika2; ./suika

clean:
	rm -rf *~ *.o suika

erase:
	rm -rf libroot

lines:
	cat ../../src/*.[chm] | wc -l
	grep -E '/\*|\*/|^([ \t]+\*)' ../../src/*.[cm] | wc -l
	grep 'assert' ../../src/*.[chm] | wc -l
