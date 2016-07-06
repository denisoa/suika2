sse.o: ../../src/sse.c
	$(CC) -c $(CPPFLAGS) -O3 $(CFLAGS) -msse $<

sse2.o: ../../src/sse2.c
	$(CC) -c $(CPPFLAGS) -O3 $(CFLAGS) -msse2 $<

sse3.o: ../../src/sse3.c
	$(CC) -c $(CPPFLAGS) -O3 $(CFLAGS) -msse3 $<

sse41.o: ../../src/sse41.c
	$(CC) -c $(CPPFLAGS) -O3 $(CFLAGS) -msse4.1 $<

sse42.o: ../../src/sse42.c
	$(CC) -c $(CPPFLAGS) -O3 $(CFLAGS) -msse4.2 $<

avx.o: ../../src/avx.c
	$(CC) -c $(CPPFLAGS) -O3 $(CFLAGS) -mavx $<

avx2.o: ../../src/avx2.c
	$(CC) -c $(CPPFLAGS) -O3 $(CFLAGS) -mavx2 $<

avx512.o: ../../src/avx512.c
	$(CC) -c $(CPPFLAGS) -O3 $(CFLAGS) -mavx512f $<
