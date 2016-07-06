/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika 2
 * Copyright (C) 2001-2016, TABATA Keiichi. All rights reserved.
 */

/*
 * [Changes]
 *  2016-06-11 作成
 */

#include "types.h"

/*
 * SSEバージョニングを行う場合のみ
 */
#ifdef SSE_VERSIONING

/* 256bit境界にする */
#define SSE_ALIGN	(32)

/* 関数ディスパッチ */
#ifdef X86_64
#define SSE_DISPATCH(f)					\
	do {						\
		if (has_avx512)				\
			avx512_ ## f;			\
		else if (has_avx2)			\
			avx2_ ## f;			\
		else if (has_avx)			\
			avx_ ## f;			\
		else if (has_sse42)			\
			sse42_ ## f;			\
		else if (has_sse41)			\
			sse41_ ## f;			\
		else if (has_sse3)			\
			sse3_ ## f;			\
		else					\
			sse2_ ## f;			\
	} while(0);
#else
#define SSE_DISPATCH(f)					\
	do {						\
		if (has_avx512) 			\
			avx512_ ## f;			\
		else if (has_avx2)			\
			avx2_ ## f;			\
		else if (has_avx)			\
			avx_ ## f;			\
		else if (has_sse42)			\
			sse42_ ## f;			\
		else if (has_sse41)			\
			sse41_ ## f;			\
		else if (has_sse3)			\
			sse3_ ## f;			\
		else if (has_sse2)			\
			sse2_ ## f;			\
		else					\
			novec_ ## f;			\
	} while(0);
#endif

/* CPUIDフラグ */
extern bool has_avx512;
extern bool has_avx2;
extern bool has_avx;
extern bool has_sse42;
extern bool has_sse41;
extern bool has_sse3;
extern bool has_sse2;
extern bool has_sse;
extern bool has_mmx;

/* CPUID命令でフラグを取得する */
void x86_check_cpuid_flags(void);

#endif	/* SSE_VERSIONING */
