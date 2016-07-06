/* -*- coding: utf-8-with-signature; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika 2
 * Copyright (C) 2001-2016, TABATA Keiichi. All rights reserved.
 */

/*
 * [Changes]
 *  - 2016/06/29 作成
 */

#include "suika.h"

/*
 * 変数テーブル
 */
static int32_t var_tbl[VAR_SIZE];

/*
 * 変数を取得する
 */
int32_t get_variable(int index)
{
	assert(index < VAR_SIZE);

	return var_tbl[index];
}

/*
 * 変数を設定する
 */
void set_variable(int index, int32_t val)
{
	assert(index < VAR_SIZE);

	var_tbl[index] = val;
}

/* TODO: 変数のセーブ(シリアライズ) */
