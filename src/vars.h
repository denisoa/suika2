/* -*- coding: utf-8-with-signature; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika 2
 * Copyright (C) 2001-2016, TABATA Keiichi. All rights reserved.
 */

/*
 * [Changes]
 *  - 2016/06/29 作成
 */

#ifndef SUIKA_VARS_H
#define SUIKA_VARS_H

#include "types.h"

/*
 * 変数の数
 */
#define VAR_SIZE	(256)

/* 変数を取得する */
int32_t get_variable(int index);

/* 変数を設定する */
void set_variable(int index, int32_t val);

/* TODO: 変数のセーブ */

#endif
