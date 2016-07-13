/* -*- tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika 2
 * Copyright (C) 2001-2016, TABATA Keiichi. All rights reserved.
 */

/*
 * [Changes]
 *  2016-06-11 作成
 */

#ifdef _WIN32
#include <windows.h>
#endif

#include "suika.h"
#include "cpuid.h"

bool has_avx512;
bool has_avx2;
bool has_avx;
bool has_sse42;
bool has_sse41;
bool has_sse3;
bool has_sse2;
bool has_sse;
bool has_mmx;

#ifdef _WIN32
static void clear_sse_flags_by_os_version(void);
#endif

/*
 * ベクトル命令の対応状況を調べる
 */
void x86_check_cpuid_flags(void)
{
	uint32_t a, b, c, d;

	/* CPUID命令でベクトル命令のサポートを調べる */
	asm("cpuid": "=b"(b), "=c"(c), "=d"(d): "a"(1));
	has_avx = c & (1 << 28);
	has_sse42 = c & (1 << 20);
	has_sse41 = c & (1 << 19);
	has_sse3 = c & 1;
	has_sse2 = d & (1 << 26);
	has_sse = d & (1 << 25);
	has_mmx = d & (1 << 23);
	asm("cpuid": "=b"(b) : "a"(7), "c"(0));
	has_avx512 = b & (1 << 16);
	has_avx2 = b & (1 << 5);

#ifdef WIN
	/* WindowsのバージョンによってSSEを無効化する */
	clear_sse_flags_by_os_version();
#endif

	/* XSAVE/XRSTORがOSにサポートされているかを調べる */
	if (has_avx) {
		asm("xgetbv": "=a"(a) : "c"(0));
		if ((a & 6) != 6) {
#ifndef NDEBUG
			printf("OS doesn't have XSAVE/XRSTOR support.\n");
#endif
			has_avx = false;
			has_avx2 = false;
			has_avx512 = false;
		}
	}

#ifndef NDEBUG
	printf("AVX512\t%d\n", has_avx512);
	printf("AVX2\t%d\n", has_avx2);
	printf("AVX\t%d\n", has_avx);
	printf("SSE4.2\t%d\n", has_sse42);
	printf("SSE4.1\t%d\n", has_sse41);
	printf("SSE3\t%d\n", has_sse3);
	printf("SSE2\t%d\n", has_sse2);
	printf("SSE\t%d\n", has_sse);
	printf("MMX\t%d\n", has_mmx);
#endif
}

#ifdef WIN
/*
 * Windowsのバージョンでベクトル命令を無効化する
 */
static void clear_sse_flags_by_os_version(void)
{
	OSVERSIONINFO vi;
	DWORD dwMajor, dwMinor, dwBuild;

	ZeroMemory(&vi, sizeof(OSVERSIONINFOEX));
	vi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	/* Windowsのバージョンを取得する */
	if (!GetVersionEx(&vi)) {
		has_avx2 = false;
		has_avx = false;
		has_sse42 = false;
		has_sse41 = false;
		has_sse3 = false;
		has_sse2 = false;
		return;
	}
	dwMajor = vi.dwMajorVersion;
	dwMinor = vi.dwMinorVersion;
	dwBuild = vi.dwBuildNumber;

	/* Windows 98 SEより前の場合、全てのSSE系命令を無効にする */
	if (dwMajor < 4 ||
	    (dwMajor == 4 && dwMinor < 10) ||
	    (dwMajor == 4 && dwMinor == 10 && dwBuild < 2222)) {
		has_sse42 = false;
		has_sse41 = false;
		has_sse3 = false;
		has_sse2 = false;
		has_sse = false;
	}
}
#endif
