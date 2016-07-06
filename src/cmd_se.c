/* -*- coding: utf-8-with-signature; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika 2
 * Copyright (C) 2001-2016, TABATA Keiichi. All rights reserved.
 */

/*
 * [Changes]
 *  - 2016/07/02 作成
 */

#include "suika.h"

/*
 * seコマンド
 */
bool se_command(void)
{
	struct wave *w;
	const char *fname;

	/* パラメータを取得する */
	fname = get_string_param(SE_PARAM_FILE);

	/* 停止の指示でない場合 */
	w = NULL;
	if (strcmp(fname, "stop") != 0) {
		/* PCMストリームをオープンする */
		w = create_wave_from_file(SE_DIR, fname, false);
		if (w == NULL) {
			log_script_exec_footer();
			return false;
		}
	}

	/* 再生を開始する */
	set_mixer_input(SE_STREAM, w);

	/* 次のコマンドへ移動する */
	return move_to_next_command();
}
