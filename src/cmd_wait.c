﻿/* -*- coding: utf-8-with-signature; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika 2
 * Copyright (C) 2001-2016, TABATA Keiichi. All rights reserved.
 */

/*
 * [Changes]
 *  - 2016/06/22 作成
 */

#include "suika.h"

static bool waiting;
static float span;
static stop_watch_t sw;

/*
 * waitコマンド
 */
bool wait_command(void)
{
	/* 初期化処理を行う */
	if (!waiting) {
		waiting = true;

		/* パラメータを取得する */
		span = get_float_param(WAIT_PARAM_SPAN);

		/* 時間の計測を開始する */
		reset_stop_watch(&sw);
	}

	/* 時間が経過した場合か、入力があった場合 */
	if ((float)get_stop_watch_lap(&sw) / 1000.0f >= span ||
	    is_control_pressed || is_return_pressed ||
	    is_left_button_pressed) {
		waiting = false;

		/* 次のコマンドへ移動する */
		return move_to_next_command();
	}

	/* waitコマンドを継続する */
	return true;
}
