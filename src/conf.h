﻿/* -*- coding: utf-8-with-signature; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika 2
 * Copyright (C) 2001-2016, TABATA Keiichi. All rights reserved.
 */

/*
 * [Changes]
 *  - 2016/06/25 作成
 */

#ifndef SUIKA_PROP_H
#define SUIKA_PROP_H

#include "types.h"

/*
 * ウィンドウの設定
 */
extern char *conf_window_title;
extern int conf_window_width;
extern int conf_window_height;
extern bool conf_window_white;

/*
 * フォントの設定
 */
extern char *conf_font_file;
extern int conf_font_size;
extern int conf_font_color_r;
extern int conf_font_color_g;
extern int conf_font_color_b;

/*
 * 名前ボックスの設定
 */
extern char *conf_namebox_file;
extern int conf_namebox_x;
extern int conf_namebox_y;
extern int conf_namebox_margin_left;
extern int conf_namebox_margin_top;

/*
 * メッセージボックスの設定
 */
extern char *conf_msgbox_file;
extern int conf_msgbox_x;
extern int conf_msgbox_y;
extern int conf_msgbox_margin_left;
extern int conf_msgbox_margin_top;
extern int conf_msgbox_margin_right;
extern int conf_msgbox_margin_line;
extern float conf_msgbox_speed;

/*
 * クリックアニメーションの設定
 */
extern char *conf_click_file;
extern int conf_click_x;
extern int conf_click_y;
extern float conf_click_interval;

/*
 * 選択肢ボックスの設定
 */
extern char *conf_selbox_bg_file;
extern char *conf_selbox_fg_file;
extern int conf_selbox_x;
extern int conf_selbox_y;
extern int conf_selbox_margin_y;

/*
 * セーブ・ロード画面の設定
 */
extern char *conf_save_bg_file;
extern char *conf_save_fg_file;
extern int conf_save_save_x;
extern int conf_save_save_y;
extern int conf_save_save_width;
extern int conf_save_save_height;
extern int conf_save_load_x;
extern int conf_save_load_y;
extern int conf_save_load_width;
extern int conf_save_load_height;
extern int conf_save_prev_x;
extern int conf_save_prev_y;
extern int conf_save_prev_width;
extern int conf_save_prev_height;
extern int conf_save_next_x;
extern int conf_save_next_y;
extern int conf_save_next_width;
extern int conf_save_next_height;
extern int conf_save_data_width;
extern int conf_save_data_height;
extern int conf_save_data_margin_left;
extern int conf_save_data_margin_top;
extern int conf_save_data1_x;
extern int conf_save_data1_y;
extern int conf_save_data2_x;
extern int conf_save_data2_y;
extern int conf_save_data3_x;
extern int conf_save_data3_y;
extern int conf_save_data4_x;
extern int conf_save_data4_y;
extern int conf_save_data5_x;
extern int conf_save_data5_y;
extern int conf_save_data6_x;
extern int conf_save_data6_y;

/*
 * ヒストリ画面の設定
 */
extern int conf_history_color_r;
extern int conf_history_color_g;
extern int conf_history_color_b;
extern int conf_history_color_a;
extern int conf_history_margin_line;
extern int conf_history_margin_left;
extern int conf_history_margin_top;
extern int conf_history_margin_right;
extern int conf_history_margin_bottom;

/* コンフィグの初期化処理を行う */
bool init_conf(void);

/* コンフィグの終了処理を行う */
void cleanup_conf(void);

#endif
