﻿/* -*- coding: utf-8-with-signature; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika 2
 * Copyright (C) 2001-2016, TABATA Keiichi. All rights reserved.
 */

/*
 * セーブ画面とセーブ・ロード実行
 *
 * [Changes]
 *  - 2016/06/30 作成
 */

#include "suika.h"

/* セーブデータ数 */
#define SAVE_SLOTS	(30)

/* 1画面あたりのセーブデータ数 */
#define PAGE_SLOTS	(6)

/* セーブ画面のページ数 */
#define SAVE_PAGES	(SAVE_SLOTS / PAGE_SLOTS)

/* ボタンの数 */
#define BUTTON_COUNT	(10)

/* ボタンのインデックス */
#define BUTTON_ONE	(0)
#define BUTTON_TWO	(1)
#define BUTTON_THREE	(2)
#define BUTTON_FOUR	(3)
#define BUTTON_FIVE	(4)
#define BUTTON_SIX	(5)
#define BUTTON_PREV	(6)
#define BUTTON_NEXT	(7)
#define BUTTON_SAVE	(8)
#define BUTTON_LOAD	(9)

/* セーブデータの日付 */
static time_t save_time[SAVE_SLOTS];

/* ボタンの座標 */
static struct button {
	int x;
	int y;
	int w;
	int h;
} button[BUTTON_COUNT];

/* ロードが実行された直後であるか */
static bool load_flag;

/* セーブ画面から復帰した直後であるか */
static bool restore_flag;

/* セーブ画面が有効であるか */
static bool is_save_mode_enabled;

/* ロード専用モードであるか */
static bool is_load_only_mode;

/* 最初の描画であるか */
static bool is_first_frame;

/* ページ */
static int page;

/* ポイントされている項目のインデックス */
static int pointed_index;

/* 選択が固定されているか */
static int selected_index;

/* 前方参照 */
static void load_button_conf(void);
static void load_save_time(void);
static void draw_page(int *x, int *y, int *w, int *h);
static int get_pointed_index(void);
static void draw_all_text_items(void);
static void draw_text_item(int x, int y, const char *text);
static void update_pointed_index(int *x, int *y, int *w, int *h);
static void process_left_press(int new_pointed_index, int *x, int *y, int *w,
			       int *h);
static void process_left_press_save_button(int new_pointed_index, int *x,
					   int *y, int *w, int *h);
static bool process_save(void);
static bool serialize_command(struct wfile *wf);
static bool serialize_stage(struct wfile *wf);
static bool serialize_bgm(struct wfile *wf);
static bool serialize_vars(struct wfile *wf);
static bool process_load(void);
static bool deserialize_command(struct rfile *rf);
static bool deserialize_stage(struct rfile *rf);
static bool deserialize_bgm(struct rfile *rf);
static bool deserialize_vars(struct rfile *rf);

/*
 * 初期化
 */

/*
 * セーブデータに関する初期化処理を行う
 */
bool init_save(void)
{
	/* 再利用時のための初期化を行う */
	load_flag = false;
	restore_flag = false;
	is_save_mode_enabled = false;

	/* コンフィグからボタンの位置と大きさをロードする */
	load_button_conf();

	/* セーブデータからセーブ時刻を取得する */
	load_save_time();

	return true;
}

/* コンフィグからボタンの位置と大きさをロードする */
static void load_button_conf(void)
{
	button[BUTTON_ONE].x = conf_save_data1_x;
	button[BUTTON_ONE].y = conf_save_data1_y;
	button[BUTTON_ONE].w = conf_save_data_width;
	button[BUTTON_ONE].h = conf_save_data_height;

	button[BUTTON_TWO].x = conf_save_data2_x;
	button[BUTTON_TWO].y = conf_save_data2_y;
	button[BUTTON_TWO].w = conf_save_data_width;
	button[BUTTON_TWO].h = conf_save_data_height;

	button[BUTTON_THREE].x = conf_save_data3_x;
	button[BUTTON_THREE].y = conf_save_data3_y;
	button[BUTTON_THREE].w = conf_save_data_width;
	button[BUTTON_THREE].h = conf_save_data_height;

	button[BUTTON_FOUR].x = conf_save_data4_x;
	button[BUTTON_FOUR].y = conf_save_data4_y;
	button[BUTTON_FOUR].w = conf_save_data_width;
	button[BUTTON_FOUR].h = conf_save_data_height;

	button[BUTTON_FIVE].x = conf_save_data5_x;
	button[BUTTON_FIVE].y = conf_save_data5_y;
	button[BUTTON_FIVE].w = conf_save_data_width;
	button[BUTTON_FIVE].h = conf_save_data_height;

	button[BUTTON_SIX].x = conf_save_data6_x;
	button[BUTTON_SIX].y = conf_save_data6_y;
	button[BUTTON_SIX].w = conf_save_data_width;
	button[BUTTON_SIX].h = conf_save_data_height;

	button[BUTTON_PREV].x = conf_save_prev_x;
	button[BUTTON_PREV].y = conf_save_prev_y;
	button[BUTTON_PREV].w = conf_save_prev_width;
	button[BUTTON_PREV].h = conf_save_prev_height;

	button[BUTTON_NEXT].x = conf_save_next_x;
	button[BUTTON_NEXT].y = conf_save_next_y;
	button[BUTTON_NEXT].w = conf_save_next_width;
	button[BUTTON_NEXT].h = conf_save_next_height;

	button[BUTTON_SAVE].x = conf_save_save_x;
	button[BUTTON_SAVE].y = conf_save_save_y;
	button[BUTTON_SAVE].w = conf_save_save_width;
	button[BUTTON_SAVE].h = conf_save_save_height;

	button[BUTTON_LOAD].x = conf_save_load_x;
	button[BUTTON_LOAD].y = conf_save_load_y;
	button[BUTTON_LOAD].w = conf_save_load_width;
	button[BUTTON_LOAD].h = conf_save_load_height;
}

/*
 * セーブデータに関する終了処理を行う
 */
void cleanup_save(void)
{
}

/*
 * コマンドからの確認
 */

/*
 * コマンドがロードによって開始されたかを確認する
 */
bool check_load_flag(void)
{
	bool ret;

	ret = load_flag;
	load_flag = false;

	return ret;
}

/* コマンドがセーブ画面から復帰したかを確認する */
bool check_restore_flag(void)
{
	bool ret;

	ret = restore_flag;
	restore_flag = false;

	return ret;
}

/*
 * セーブ画面
 */

/*
 * セーブ画面を開始する
 */
void start_save_mode(bool load_only)
{
	/* セーブ画面を開始する */
	is_save_mode_enabled = true;

	/* ロード専用モードであるかを記憶する */
	is_load_only_mode = load_only;

	/* 最初のフレームである */
	is_first_frame = true;

	/* 選択項目を無効とする */
	pointed_index = -1;

	/* セーブデータの選択を無効とする */
	selected_index = -1;

	/* 0ページ目を表示する */
	page = 0;
}

/* セーブ画面を終了する */
static void stop_save_mode(int *x, int *y, int *w, int *h)
{
	/* セーブ画面を終了する */
	is_save_mode_enabled = false;

	/* ステージを再描画する */
	draw_stage();

	/* ステージ全体をウィンドウに転送する */
	*x = 0;
	*y = 0;
	*w = conf_window_width;
	*h = conf_window_height;
}

/*
 * セーブ画面が有効であるかを返す
 */
bool is_save_mode(void)
{
	return is_save_mode_enabled;
}

/*
 * セーブ画面の1フレームを実行する
 */
void run_save_mode(int *x, int *y, int *w, int *h)
{
	/* 最初のフレームを実行する */
	if (is_first_frame) {
		draw_page(x, y, w, h);
		is_first_frame = false;
		return;
	}

	/* 右クリックされた場合、セーブをキャンセルする */
	if (is_right_button_pressed) {
		stop_save_mode(x, y, w, h);
		restore_flag = true;
		return;
	}

	/* ポイントされている項目を更新する */
	update_pointed_index(x, y, w, h);
}

/* ページの描画を行う */
static void draw_page(int *x, int *y, int *w, int *h)
{
	/* ステージのレイヤの背景を描画する */
	/* FIXME: draw_image_to_fi/foに変更するか？ */
	clear_save_stage();

	/* セーブデータのテキストを描画する */
	draw_all_text_items();

	/* 現在選択されている項目を取得する */
	pointed_index = get_pointed_index();
	if (pointed_index != -1) {
		/* 選択されているボタンがある場合 */
		draw_stage_with_buttons(button[pointed_index].x,
					button[pointed_index].y,
					button[pointed_index].w,
					button[pointed_index].h,
					0, 0, 0, 0);
	} else {
		/* 選択されているボタンがない場合 */
		draw_stage_with_buttons(0, 0, 0, 0, 0, 0, 0, 0);
	}

	*x = 0;
	*y = 0;
	*w = conf_window_width;
	*h = conf_window_height;
}

/* ポイントされているボタンを返す */
static int get_pointed_index(void)
{
	int i, pointed;

	/* 領域を元にポイントされているボタンを求める */
	pointed = -1;
	for (i = 0; i < BUTTON_COUNT; i++) {
		if (mouse_pos_x >= button[i].x &&
		    mouse_pos_x < button[i].x + button[i].w &&
		    mouse_pos_y >= button[i].y &&
		    mouse_pos_y < button[i].y + button[i].h) {
			pointed = i;
			break;
		}
	}
	if (pointed == -1)
		return -1;

	/* 最初のページの場合、BUTTON_PREVを無効にする */
	if (page == 0 && pointed == BUTTON_PREV)
		return -1;

	/* 最後のページの場合、BUTTON_NEXTを無効にする */
	if (page == SAVE_PAGES - 1 && pointed == BUTTON_NEXT)
		return -1;

	/* ロード専用モードの場合、BUTTON_SAVEを無効にする */
	if (is_load_only_mode && pointed == BUTTON_SAVE)
		return -1;

	/* 選択されたボタンがない場合、BUTTON_SAVEとBUTTON_LOADを無効にする */
	if (selected_index == -1 && pointed == BUTTON_SAVE)
		return -1;
	if (selected_index == -1 && pointed == BUTTON_LOAD)
		return -1;

	/* 選択されたボタンにセーブデータがない場合、BUTTON_LOADを無効にする */
	if (selected_index != -1 && pointed == BUTTON_LOAD &&
	    save_time[selected_index - BUTTON_ONE] == 0)
		return -1;

	/* ポイントされたボタンを返す */
	return pointed;
}

/* セーブデータのテキストを描画する */
static void draw_all_text_items(void)
{
	struct tm *timeptr;
	char text[128];
	int i, j;

	/* 先頭のセーブデータの番号を求める */
	j = page * PAGE_SLOTS;

	/* 6つのセーブボタンについて描画する */
	for (i = BUTTON_ONE; i <= BUTTON_SIX; i++, j++) {
		if (save_time[j] == 0) {
			snprintf(text, sizeof(text), "[%02d] NO DATA", j + 1);
		} else {
			timeptr = localtime(&save_time[j]);
			snprintf(text, sizeof(text), "[%02d] ", j + 1);
			strftime(&text[5], sizeof(text), "%m/%d %H:%M",
				 timeptr);
		}

		draw_text_item(button[i].x + conf_save_data_margin_left,
			       button[i].y + conf_save_data_margin_top,
			       text);
	}
}

/* セーブデータのテキストを描画する */
static void draw_text_item(int x, int y, const char *text)
{
	uint32_t wc;
	int mblen;

	/* 1文字ずつ描画する */
	while (*text != '\0') {
		/* 描画する文字を取得する */
		mblen = utf8_to_utf32(text, &wc);
		if (mblen == -1)
			return;

		/* 描画する */
		x += draw_char_on_fo_fi(x, y, wc);

		/* 次の文字へ移動する */
		text += mblen;
	}
}

/*
 * ポイントされている項目を更新する
 */
void update_pointed_index(int *x, int *y, int *w, int *h)
{
	int new_pointed_index;

	/* 現在ポイントされている項目を取得する */
	new_pointed_index = get_pointed_index();

	/* 左クリックされた場合 */
	if (is_left_button_pressed) {
		process_left_press(new_pointed_index, x, y, w, h);
		return;
	}

	/* 前回ポイントされていた項目と同じ場合は何もしない */
	if (new_pointed_index == pointed_index)
		return;

	/* 選択されたボタンとポイントされたボタンをバックイメージに描画する */
	if (new_pointed_index != -1) {
		/* 選択中のボタンがある場合 */
		if (selected_index != -1 &&
		    selected_index != new_pointed_index) {
			draw_stage_with_buttons(
				button[new_pointed_index].x,
				button[new_pointed_index].y,
				button[new_pointed_index].w,
				button[new_pointed_index].h,
				button[selected_index].x,
				button[selected_index].y,
				button[selected_index].w,
				button[selected_index].h);
		} else {
			/* 選択中のボタンがない場合 */
			draw_stage_with_buttons(
				button[new_pointed_index].x,
				button[new_pointed_index].y,
				button[new_pointed_index].w,
				button[new_pointed_index].h,
				0, 0, 0, 0);
		}
	} else {
		/* 選択中のボタンがある場合 */
		if (selected_index != -1) {
			draw_stage_with_buttons(
				button[selected_index].x,
				button[selected_index].y,
				button[selected_index].w,
				button[selected_index].h,
				0, 0, 0, 0);
		} else {
			/* 選択中のボタンがない場合 */
			draw_stage_with_buttons(0, 0, 0, 0, 0, 0, 0, 0);
		}
	}

	/* ウィンドウの更新領域を求める */
	if (new_pointed_index != -1) {
		union_rect(x, y, w, h,
			   button[new_pointed_index].x,
			   button[new_pointed_index].y,
			   button[new_pointed_index].w,
			   button[new_pointed_index].h,
			   button[pointed_index].x,
			   button[pointed_index].y,
			   button[pointed_index].w,
			   button[pointed_index].h);
	} else {
		union_rect(x, y, w, h, 0, 0, 0, 0,
			   button[pointed_index].x,
			   button[pointed_index].y,
			   button[pointed_index].w,
			   button[pointed_index].h);
	}

	/* ポイントされている項目を変更する */
	pointed_index = new_pointed_index;
}

/* 左ボタンのクリックを処理する */
static void process_left_press(int new_pointed_index, int *x, int *y, int *w,
			       int *h)
{
	/* ポイントされている項目がない場合 */
	if (new_pointed_index == -1)
		return;

	/* セーブデータのボタンの場合、選択する */
	if (new_pointed_index >= BUTTON_ONE &&
	    new_pointed_index <= BUTTON_SIX) {
		process_left_press_save_button(new_pointed_index, x, y, w, h);
		return;
	}

	/* 前ページ、次ページボタンの場合 */
	if (new_pointed_index == BUTTON_PREV ||
	    new_pointed_index == BUTTON_NEXT) {
		page += new_pointed_index == BUTTON_PREV ? -1 : 1;
		selected_index = -1;
		draw_page(x, y, w, h);
		return;
	}

	/* セーブボタンの場合 */
	if (new_pointed_index == BUTTON_SAVE) {
		process_save();
		stop_save_mode(x, y, w, h);
		restore_flag = true;
	}

	/* ロードボタンの場合 */
	if (new_pointed_index == BUTTON_LOAD) {
		process_load();
		stop_save_mode(x, y, w, h);
	}
}

/* セーブボタン上での左クリックを処理する */
static void process_left_press_save_button(int new_pointed_index, int *x,
					   int *y, int *w, int *h)
{
	/* バックイメージに新しい選択ボタンを描画する */
	draw_stage_with_buttons(
		button[new_pointed_index].x,
		button[new_pointed_index].y,
		button[new_pointed_index].w,
		button[new_pointed_index].h,
		0, 0, 0, 0);

	/* ウィンドウの更新領域を求める */
	union_rect(x, y, w, h,
		   button[new_pointed_index].x,
		   button[new_pointed_index].y,
		   button[new_pointed_index].w,
		   button[new_pointed_index].h,
		   button[selected_index].x,
		   button[selected_index].y,
		   button[selected_index].w,
		   button[selected_index].h);

	/* 選択されている項目を更新する */
	selected_index = new_pointed_index;

	/* ポイントされている項目を更新する */
	pointed_index = new_pointed_index;
}

/*
 * セーブの実際の処理
 */

/* セーブを処理する */
static bool process_save(void)
{
	char file[128];
	struct wfile *wf;
	uint64_t t;
	int index;
	bool success;

	/* セーブディレクトリを作成する */
	make_sav_dir();

	/* ファイル名を求める */
	index = page * PAGE_SLOTS + (selected_index - BUTTON_ONE);
	snprintf(file, sizeof(file), "%03d.sav", index);

	/* ファイルを開く */
	wf = open_wfile(SAVE_DIR, file);
	if (wf == NULL)
		return false;

	success = false;
	do {
		/* 日付を書き込む */
		t = (uint64_t)time(NULL);
		if (write_wfile(wf, &t, sizeof(t)) < sizeof(t))
			break;

		/* コマンド位置のシリアライズを行う */
		if (!serialize_command(wf))
			break;

		/* ステージのシリアライズを行う */
		if (!serialize_stage(wf))
			break;

		/* BGMのシリアライズを行う */
		if (!serialize_bgm(wf))
			break;

		/* 変数のシリアライズを行う */
		if (!serialize_vars(wf))
			break;

		/* 成功 */
		success = true;
	} while (0);

	/* ファイルをクローズする */
	close_wfile(wf);

	/* 時刻を保存する */
	if (success)
		save_time[index] = (time_t)t;

	return success;
}

/* コマンド位置をシリアライズする */
static bool serialize_command(struct wfile *wf)
{
	const char *s;
	int n;
	
	s = get_script_file_name();
	if (write_wfile(wf, s, strlen(s) + 1) < strlen(s) + 1)
		return false;

	n = get_command_index();
	if (write_wfile(wf, &n, sizeof(n)) < sizeof(n))
		return false;

	return true;
}

/* ステージをシリアライズする */
static bool serialize_stage(struct wfile *wf)
{
	const char *s;
	int i, m, n;

	s = get_bg_file_name();
	if (write_wfile(wf, s, strlen(s) + 1) < strlen(s) + 1)
		return false;

	for (i = CH_BACK; i <= CH_CENTER; i++) {
		get_ch_position(i, &m, &n);
		if (write_wfile(wf, &m, sizeof(m)) < sizeof(m))
			return false;
		if (write_wfile(wf, &n, sizeof(n)) < sizeof(n))
			return false;

		s = get_ch_file_name(i);
		if (write_wfile(wf, s, strlen(s) + 1) < strlen(s) + 1)
			return false;
	}
	return true;
}

/* BGMをシリアライズする */
static bool serialize_bgm(struct wfile *wf)
{
	const char *s;

	s = get_bgm_file_name();
	if (write_wfile(wf, s, strlen(s) + 1) < strlen(s) + 1)
		return false;

	return true;
}

/* 変数をシリアライズする */
static bool serialize_vars(struct wfile *wf)
{
	int i, n;

	for (i = 0; i < VAR_SIZE; i++) {
		n = get_variable(i);
		if (write_wfile(wf, &n, sizeof(n)) < sizeof(n))
			return false;
	}
	return true;
}

/*
 * ロードの実際の処理
 */

/* ロードを処理する */
static bool process_load(void)
{
	char s[128];
	struct rfile *rf;
	uint64_t t;
	int index;
	bool success;

	/* ファイル名を求める */
	index = page * PAGE_SLOTS + (selected_index - BUTTON_ONE);
	snprintf(s, sizeof(s), "%03d.sav", index);

	/* ファイルを開く */
	rf = open_rfile(SAVE_DIR, s, false);
	if (rf == NULL)
		return false;

	success = false;
	do {
		/* 日付を読み込む */
		if (read_rfile(rf, &t, sizeof(t)) < sizeof(t))
			break;

		/* コマンド位置のデシリアライズを行う */
		if (!deserialize_command(rf))
			break;

		/* ステージのデシリアライズを行う */
		if (!deserialize_stage(rf))
			break;

		/* BGMのデシリアライズを行う */
		if (!deserialize_bgm(rf))
			break;

		/* 変数のデシリアライズを行う */
		if (!deserialize_vars(rf))
			break;
		
		/* ロードが実行された直後であることをマークする */
		load_flag = true;

		/* 成功 */
		success = true;
	} while (0);

	/* ファイルをクローズする */
	close_rfile(rf);

	/* 名前ボックス、メッセージボックス、選択ボックスを非表示とする */
	show_namebox(false);
	show_msgbox(false);
	show_selbox(false);

	return success;
}

/* コマンド位置のデシリアライズを行う */
static bool deserialize_command(struct rfile *rf)
{
	char s[1024];
	int n;

	if (gets_rfile(rf, s, sizeof(s)) == NULL)
		return false;

	if (!load_script(s))
		return false;

	if (read_rfile(rf, &n, sizeof(n)) < sizeof(n))
		return false;

	if (!move_to_command_index(n))
		return false;

	return true;
}

/* ステージのデシリアライズを行う */
static bool deserialize_stage(struct rfile *rf)
{
	char s[1024];
	struct image *img;
	int m, n, i;

	if (gets_rfile(rf, s, sizeof(s)) == NULL)
		return false;

	set_bg_file_name(s);

	if (strcmp(s, "none") != 0) {
		img = create_image_from_file(BG_DIR, s);
		if (img == NULL) {
			log_dir_file_open(BG_DIR, s);
			return false;
		}
	} else {
		img = NULL;
	}

	change_bg_immediately(img);

	for (i = CH_BACK; i <= CH_CENTER; i++) {
		if (read_rfile(rf, &m, sizeof(m)) < sizeof(n))
			return false;
		if (read_rfile(rf, &n, sizeof(n)) < sizeof(m))
			return false;
		if (gets_rfile(rf, s, sizeof(s)) == NULL)
			return false;

		set_ch_file_name(i, s);

		if (strcmp(s, "none") != 0) {
			img = create_image_from_file(CH_DIR, s);
			if (img == NULL) {
				log_dir_file_open(CH_DIR, s);
				return false;
			}
		} else {
			img = NULL;
		}

		change_ch_immediately(i, img, m, n);
	}
	return true;
}

/* BGMをデシリアライズする */
static bool deserialize_bgm(struct rfile *rf)
{
	char s[1024];
	struct wave *w;

	if (gets_rfile(rf, s, sizeof(s)) == NULL)
		return false;

	set_bgm_file_name(s);

	if (strcmp(s, "none") != 0) {
		w = create_wave_from_file(BGM_DIR, s, true);
		if (w == NULL)
			return false;
	} else {
		w = NULL;
	}

	set_mixer_input(BGM_STREAM, w);

	return true;
}

/* 変数をデシリアライズする */
static bool deserialize_vars(struct rfile *rf)
{
	int i, n;

	for (i = 0; i < VAR_SIZE; i++) {
		if (read_rfile(rf, &n, sizeof(n)) < sizeof(n))
			return false;
		set_variable(i, n);
	}
	return true;
}

/* セーブデータからセーブ時刻を読み込む */
static void load_save_time(void)
{
	struct rfile *rf;
	char buf[128];
	uint64_t t;
	int i;

	for (i = 0; i < SAVE_SLOTS; i++) {
		/* セーブデータファイルを開く */
		snprintf(buf, sizeof(buf), "%03d.sav", i);
		rf = open_rfile(SAVE_DIR, buf, true);
		if (rf != NULL) {
			/* セーブ時刻を取得する */
			if (read_rfile(rf, &t, sizeof(t)) == sizeof(t))
				save_time[i] = (time_t)t;
			close_rfile(rf);
		}
	}
}
