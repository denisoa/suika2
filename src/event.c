/* -*- coding: utf-8-with-signature; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika 2
 * Copyright (C) 2001-2016, TABATA Keiichi. All rights reserved.
 */

/*
 * イベントハンドラ
 *  - platform.hの実装から呼び出される
 *
 * [Changes]
 *  - 2016/05/27 作成
 *  - 2016/06/22 分割
 */

#include "suika.h"

/*
 * プラットフォーム非依存な初期化処理を行う
 */
bool on_event_init(void)
{
	/* 文字レンダリングエンジンの初期化処理を行う */
	if (!init_glyph())
		return false;

	/* ミキサの初期化処理を行う */
	init_mixer();

	/* ステージの初期化処理を行う */
	if (!init_stage())
		return false;

	/* セーブデータ関連の初期化を行う */
	init_save();

	/* 初期スクリプトをロードする */
	if (!init_script())
		return false;

	return true;
}

/*
 * プラットフォーム非依存な終了処理を行う
 */
void on_event_cleanup(void)
{
	/* スクリプトの終了処理を行う */
	cleanup_script();

	/* デーブデータ関連の終了処理を行う */
	cleanup_save();

	/* ステージの終了処理を行う */
	cleanup_stage();

	/* ミキサの終了処理を行う */
	cleanup_mixer();

	/* 文字レンダリングエンジンの終了処理を行う */
	cleanup_glyph();
}

/*
 * 再描画時に呼び出される
 */
bool on_event_frame(int *x, int *y, int *w, int *h)
{
	/* デフォルトの書き換え領域をなしとする */
	*x = *y = *w = *h = 0;
	
	/* ゲームループの中身を実行する */
	if (!game_loop_iter(x, y, w, h)) {
		/* アプリケーションを終了する */
		return false;
	}

	/* アプリケーションを続行する */
	return true;
}

/*
 * キー押下時に呼び出される
 */
void on_event_key_press(int key)
{
	switch(key) {
	case KEY_CONTROL:
		is_control_pressed = true;
		break;
	case KEY_SPACE:
		is_space_pressed = true;
		break;
	case KEY_RETURN:
		is_return_pressed = true;
		break;
	case KEY_ESCAPE:
		is_escape_pressed = true;
		break;
	case KEY_UP:
		is_up_pressed = true;
		break;
	case KEY_DOWN:
		is_down_pressed = true;
		break;
	case KEY_PAGE_UP:
		is_page_up_pressed = true;
		break;
	case KEY_PAGE_DOWN:
		is_page_down_pressed = true;
		break;
	default:
		assert(0);
		break;
	}
}

/*
 * キー解放時に呼び出される
 */
void on_event_key_release(int key)
{
	switch(key) {
	case KEY_CONTROL:
		is_control_pressed = false;
		break;
	case KEY_SPACE:
		is_space_pressed = false;
		break;
	case KEY_RETURN:
	case KEY_ESCAPE:
	case KEY_UP:
	case KEY_DOWN:
	case KEY_PAGE_UP:
	case KEY_PAGE_DOWN:
		/* これらのキーはフレームごとに解放されたことにされる */
		break;
	default:
		assert(0);
		break;
	}
}

/*
 * マウス押下時に呼び出される
 */
void on_event_mouse_press(int button, UNUSED(int x), UNUSED(int y))
{
	if (button == MOUSE_LEFT)
		is_left_button_pressed = true;
	else
		is_right_button_pressed = true;
}

/*
 * マウス解放時に呼び出される
 */
void on_event_mouse_release(UNUSED(int button), UNUSED(int x), UNUSED(int y))
{
	/* 1フレーム内の解放を無視する */
}

/*
 * マウス移動時に呼び出される
 */
void on_event_mouse_move(int x, int y)
{
	mouse_pos_x = x;
	mouse_pos_y = y;
}

/*
 * マウススクロール時に呼び出される
 */
void on_event_mouse_scroll(UNUSED(int n))
{
}
