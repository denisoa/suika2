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
static void reset_flags_by_win_ver(void);
#endif

/*
 * ベクトル命令の対応状況を調べる
 */
void x86_check_cpuid_flags(void)
{
	uint32_t b, c, d;

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

#ifdef _WIN32
	reset_flags_by_win_ver();
#endif

	printf("AVX512\t%d\n", has_avx512);
	printf("AVX2\t%d\n", has_avx2);
	printf("AVX\t%d\n", has_avx);
	printf("SSE4.2\t%d\n", has_sse42);
	printf("SSE4.1\t%d\n", has_sse41);
	printf("SSE3\t%d\n", has_sse3);
	printf("SSE2\t%d\n", has_sse2);
	printf("SSE\t%d\n", has_sse);
	printf("MMX\t%d\n", has_mmx);
}

#ifdef _WIN32
/*
 * Windowsのバージョンごとにベクトル命令を無効化する
 */
static void reset_flags_by_win_ver()
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

	/* FIXME: AVX-512fに対応するOSバージョンは？ */

	/* Windows 7 SP1より前の場合、AVX系命令を無効にする */
	if (dwMajor < 6 ||
	    (dwMajor == 6 && dwMinor < 1) ||
	    (dwMajor == 6 && dwMinor == 1 && dwBuild < 7601)) {
		has_avx512 = false;
		has_avx2 = false;
		has_avx = false;
	}

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
