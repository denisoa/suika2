/* -*- coding: utf-8-with-signature; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika 2
 * Copyright (C) 2001-2016, TABATA Keiichi. All rights reserved.
 */

/*
 * [Changes]
 *  - 2016/06/28 作成
 */

#include "suika.h"

/*
 * 背景コマンド
 */
bool vol_command(void)
{
	const char *stream;
	float vol, span;

	stream = get_string_param(VOL_PARAM_STREAM);
	vol = get_float_param(VOL_PARAM_VOL);
	span = get_float_param(VOL_PARAM_SPAN);

	vol = vol < 0 ? 0 : vol;
	vol = vol > 1.0f ? 1.0f : vol;

	switch(stream[0]) {
	case 'b':
		set_mixer_volume(BGM_STREAM, vol, span);
		break;
	case 'v':
		set_mixer_volume(VOICE_STREAM, vol, span);
		break;
	case 's':
		set_mixer_volume(SE_STREAM, vol, span);
		break;
	}

	return move_to_next_command();
}
