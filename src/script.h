/* -*- coding: utf-8-with-signature; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika 2
 * Copyright (C) 2001-2016, TABATA Keiichi. All rights reserved.
 */

/*
 * [Changes]
 *  - 2016/06/01 作成
 */

#ifndef SUIKA_SCRIPT_H
#define SUIKA_SCRIPT_H

#include "types.h"

/* コマンド構造体 */
struct command;

/* コマンドの種類 */
enum command_type {
	COMMAND_MIN,		/* invalid value */
	COMMAND_LABEL,
	COMMAND_SERIF,
	COMMAND_MESSAGE,
	COMMAND_BG,
	COMMAND_BGM,
	COMMAND_CH,
	COMMAND_CLICK,
	COMMAND_WAIT,
	COMMAND_GOTO,
	COMMAND_LOAD,
	COMMAND_VOL,
	COMMAND_SET,
	COMMAND_IF,
	COMMAND_SELECT,
	COMMAND_SE,
	COMMAND_MENU,
	COMMAND_MAX		/* invalid value */
};

/* labelコマンドのパラメータ */
enum label_command_param {
	LABEL_PARAM_LABEL = 0,
};

/* セリフコマンドのパラメータ */
enum serif_command_param {
	SERIF_PARAM_PTR,
	SERIF_PARAM_NAME,
	SERIF_PARAM_VOICE,
	SERIF_PARAM_MESSAGE,
};

/* bgコマンドのパラメータ */
enum bg_command_param {
	BG_PARAM_FILE = 1,
	BG_PARAM_SPAN,
	BG_PARAM_METHOD,
};

/* bgmコマンドのパラメータ */
enum bgm_command_param {
	BGM_PARAM_FILE = 1,
};

/* chコマンドのパラメータ */
enum ch_command_param {
	CH_PARAM_POS = 1,
	CH_PARAM_FILE,
	CH_PARAM_SPAN,
};

/* waitコマンドのパラメータ */
enum wait_command_param {
	WAIT_PARAM_SPAN = 1,
};

/* gotoコマンドのパラメータ */
enum goto_command_param {
	GOTO_PARAM_LABEL = 1,
};

/* loadコマンドのパラメータ */
enum load_command_param {
	LOAD_PARAM_FILE = 1,
};

/* volコマンドのパラメータ */
enum vol_command_param {
	VOL_PARAM_STREAM = 1,
	VOL_PARAM_VOL,
	VOL_PARAM_SPAN,
};

/* setコマンドのパラメータ */
enum set_command_param {
	SET_PARAM_LHS = 1,
	SET_PARAM_OP,
	SET_PARAM_RHS,
};

/* ifコマンドのパラメータ */
enum if_command_param {
	IF_PARAM_LHS = 1,
	IF_PARAM_OP,
	IF_PARAM_RHS,
	IF_PARAM_LABEL,
};

/* selectコマンドのパラメータ */
enum select_command_param {
	SELECT_PARAM_LABEL1 = 1,
	SELECT_PARAM_LABEL2,
	SELECT_PARAM_LABEL3,
	SELECT_PARAM_TEXT1,
	SELECT_PARAM_TEXT2,
	SELECT_PARAM_TEXT3,
};

/* seコマンドのパラメータ */
enum se_command_param {
	SE_PARAM_FILE = 1,
};

/* menuコマンドのパラメータ */
enum menu_command_param {
	MENU_PARAM_BG_FILE = 1,
	MENU_PARAM_FG_FILE,
	MENU_PARAM_LABEL1,
	MENU_PARAM_X1,
	MENU_PARAM_Y1,
	MENU_PARAM_W1,
	MENU_PARAM_H1,
	MENU_PARAM_LABEL2,
	MENU_PARAM_X2,
	MENU_PARAM_Y2,
	MENU_PARAM_W2,
	MENU_PARAM_H2,
	MENU_PARAM_LABEL3,
	MENU_PARAM_X3,
	MENU_PARAM_Y3,
	MENU_PARAM_W3,
	MENU_PARAM_H3,
	MENU_PARAM_LABEL4,
	MENU_PARAM_X4,
	MENU_PARAM_Y4,
	MENU_PARAM_W4,
	MENU_PARAM_H4,
};

/*
 * 初期化
 */

/* 初期スクリプトを読み込む */
bool init_script(void);

/* コマンドを破棄する */
void cleanup_script(void);

/*
 * スクリプトとコマンドへの公開アクセス
 */

/* スクリプトをロードする */
bool load_script(const char *fname);

/* スクリプトファイル名を取得する */
const char *get_script_file_name(void);

/* 実行中のコマンドのインデックスを取得する(セーブ用) */
int get_command_index(void);

/* 実行中のコマンドのインデックスを取得する(ロード用) */
bool move_to_command_index(int index);

/* 次のコマンドに移動する */
bool move_to_next_command(void);

/* ラベルへ移動する */
bool move_to_label(const char *label);

/* コマンドの行番号を取得する */
int get_line_num(void);

/* コマンドの行全体を取得する */
const char *get_line_string(void);

/* コマンドのタイプを取得する */
int get_command_type(void);

/* コマンドの文字列全体を取得する */
const char *get_command_string(void);

/* 文字列のコマンドパラメータを取得する */
const char *get_string_param(int index);

/* 整数のコマンドパラメータを取得する */
int get_int_param(int index);

/* 浮動小数点数のコマンドパラメータを取得する */
float get_float_param(int index);

#endif
