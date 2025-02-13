﻿/* -*- coding: utf-8-with-signature; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika 2
 * Copyright (C) 2001-2016, TABATA Keiichi. All rights reserved.
 */

/*
 * [Changes]
 *  - 2016/06/01 作成
 */

#include "suika.h"

/* 1行の読み込みサイズ */
#define LINE_BUF_SIZE	(4096)

/*
 * コマンド配列
 */

/* コマンドの最大数 */
#define CMD_SIZE	(1000)

/* コマンドの引数の数(コマンド名も含める) */
#define PARAM_SIZE	(23)

/* コマンド配列 */
static struct command {
	int type;
	int line;
	char *text;
	char *param[PARAM_SIZE];
} cmd[CMD_SIZE];
static int cmd_size;

/*
 * 命令の種類
 */

struct insn_item {
	const char *str;	/* 命令の文字列 */
	int type;		/* コマンドのタイプ */
	int min;		/* 最小のパラメータ数 */
	int max;		/* 最大のパラメータ数 */
} insn_tbl[] = {
	{"@bg", COMMAND_BG, 1, 3},
	{"@bgm", COMMAND_BGM, 1, 1},
	{"@ch", COMMAND_CH, 1, 3},
	{"@click", COMMAND_CLICK, 0, 1},
	{"@wait", COMMAND_WAIT, 1, 1},
	{"@goto", COMMAND_GOTO, 1, 1},
	{"@load", COMMAND_LOAD, 1, 1},
	{"@vol", COMMAND_VOL, 2, 3},
	{"@set", COMMAND_SET, 3, 3},
	{"@if", COMMAND_IF, 4, 4},
	{"@select", COMMAND_SELECT, 6, 6},
	{"@se", COMMAND_SE, 1, 1},
	{"@menu", COMMAND_MENU, 7, 22},
	{"@retrospect", COMMAND_RETROSPECT, 11, 55},
};

#define INSN_TBL_SIZE	(sizeof(insn_tbl) / sizeof(struct insn_item))

/*
 * コマンド実行ポインタ
 */
static char *cur_script;
static int cur_index;

/*
 * 前方参照
 */
static bool read_script_from_file(const char *fname);
static bool parse_insn(int index, const char *fname, int line,
		       const char *buf);
static bool parse_serif(int index, const char *fname, int line,
			const char *buf);
static bool parse_message(int index, const char *fname, int line,
			  const char *buf);
static bool parse_label(int index, const char *fname, int line,
			const char *buf);

/*
 * 初期化
 */

/*
 * 初期スクリプトを読み込む
 */
bool init_script(void)
{
	if (!load_script(INIT_FILE))
		return false;

	return true;
}

/*
 * コマンドを破棄する
 */
void cleanup_script(void)
{
	int i, j;

	for (i = 0; i < CMD_SIZE; i++) {
		/* コマンドタイプをクリアする */
		cmd[i].type = COMMAND_MIN;

		/* 行の内容を解放する */
		if (cmd[i].text != NULL) {
			free(cmd[i].text);
			cmd[i].text = NULL;
		}

		/* 引数の本体を解放する */
		if (cmd[i].param[0] != NULL) {
			free(cmd[i].param[0]);
			cmd[i].param[0] = NULL;
		}

		/* 引数の参照をNULLで上書きする */
		for (j = 1; j < PARAM_SIZE; j++)
			cmd[i].param[j] = NULL;
	}

	if (cur_script != NULL) {
		free(cur_script);
		cur_script = NULL;
	}
}

/*
 * スクリプトとコマンドへの公開アクセス
 */

/*
 * スクリプトをロードする
 */
bool load_script(const char *fname)
{
	/* 現在のスクリプトを破棄する */
	cleanup_script();

	/* スクリプト名を保存する */
	cur_index = 0;
	cur_script = strdup(fname);
	if (cur_script == NULL) {
		log_memory();
		return false;
	}

	/* スクリプトファイルを読み込む */
	if (!read_script_from_file(fname))
		return false;

	/* コマンドが含まれない場合 */
	if (cmd_size == 0) {
		log_script_no_command(fname);
		return false;
	}

	return true;
}

/*
 * スクリプトファイル名を取得する
 */
const char *get_script_file_name(void)
{
	return cur_script;
}

/*
 * 実行中のコマンドのインデックスを取得する(セーブ用)
 */
int get_command_index(void)
{
	return cur_index;
}

/*
 * 実行中のコマンドのインデックスを取得する(ロード用)
 */
bool move_to_command_index(int index)
{
	if (index < 0 || index >= cmd_size)
		return false;

	cur_index = index;
	return true;
}

/*
 * 次のコマンドに移動する
 */
bool move_to_next_command(void)
{
	assert(cur_index < cmd_size);

	/* スクリプトの末尾に達した場合 */
	if (++cur_index == cmd_size)
		return false;

	return true;
}

/*
 * ラベルへ移動する
 */
bool move_to_label(const char *label)
{
	struct command *c;
	int i;

	/* ラベルを探す */
	for (i = 0; i < cmd_size; i++) {
		/* ラベルでないコマンドをスキップする */
		c = &cmd[i];
		if (c->type != COMMAND_LABEL)
			continue;

		/* ラベルがみつかった場合 */
		if (strcmp(c->param[LABEL_PARAM_LABEL], label) == 0) {
			cur_index = i + 1;

			/* スクリプトの末尾に達した場合 */
			if (cur_index == cmd_size)
				return false;

			return true;
		}
	}

	/* エラーを出力する */
	log_script_label_not_found(label);
	log_script_exec_footer();
	return false;
}

/*
 * コマンドの行番号を取得する
 */
int get_line_num(void)
{
	return cmd[cur_index].line;
}

/* コマンドの行番号を取得する */
const char *get_line_string(void)
{
	struct command *c;

	c = &cmd[cur_index];

	return c->text;
}

/*
 * コマンドのタイプを取得する
 */
int get_command_type(void)
{
	struct command *c;

	assert(cur_index < cmd_size);

	c = &cmd[cur_index];
	assert(c->type > COMMAND_MIN && c->type < COMMAND_MAX);

	return c->type;
}

/*
 * 文字列のコマンドパラメータを取得する
 */
const char *get_string_param(int index)
{
	struct command *c;

	assert(cur_index < cmd_size);
	assert(index < PARAM_SIZE);

	c = &cmd[cur_index];

	/* パラメータが省略された場合 */
	if (c->param[index] == NULL)
		return "";

	/* 文字列を返す */
	return c->param[index];
}

/*
 * 整数のコマンドパラメータを取得する
 */
int get_int_param(int index)
{
	struct command *c;

	assert(cur_index < cmd_size);
	assert(index < PARAM_SIZE);

	c = &cmd[cur_index];

	/* パラメータが省略された場合 */
	if (c->param[index] == NULL)
		return 0;

	/* 整数に変換して返す */
	return atoi(c->param[index]);
}

/*
 * 浮動小数点数のコマンドパラメータを取得する
 */
float get_float_param(int index)
{
	struct command *c;

	assert(cur_index < cmd_size);
	assert(index < PARAM_SIZE);

	c = &cmd[cur_index];

	/* パラメータが省略された場合 */
	if (c->param[index] == NULL)
		return 0.0f;

	/* 浮動小数点数に変換して返す */
	return (float)atof(c->param[index]);
}

/*
 * スクリプトファイルの読み込み
 */

/*
 * ファイルを読み込む
 */
static bool read_script_from_file(const char *fname)
{
	char buf[LINE_BUF_SIZE];
	struct rfile *rf;
	int line;
	bool result;

	/* ファイルをオープンする */
	rf = open_rfile(SCRIPT_DIR, fname, false);
	if (rf == NULL) {
		log_dir_file_open(SCRIPT_DIR, fname);
		return false;
	}

	/* 行ごとに処理する */
	cmd_size = 0;
	line = 0;
	result = true;
	while (result) {
		/* 行を読み込む */
		if (gets_rfile(rf, buf, sizeof(buf)) == NULL)
			break;

		/* 最大コマンド数をチェックする */
		if (line >= CMD_SIZE) {
			log_script_size(CMD_SIZE);
			result = false;
			break;
		}

		/* 行頭の文字で仕分けする */
		switch (buf[0]) {
		case '\0':
		case '#':
			/* 空行とコメント行を読み飛ばす */
			break;
		case '@':
			/* 命令行をパースする */
			if (!parse_insn(cmd_size, fname, line, buf))
				result = false;
			else
				cmd_size++;
			break;
		case '*':
			/* セリフ行をパースする */
			if (!parse_serif(cmd_size, fname, line, buf))
				result = false;
			else
				cmd_size++;
			break;
		case ':':
			/* ラベル行をパースする */
			if (!parse_label(cmd_size, fname, line, buf))
				result = false;
			else
				cmd_size++;
			break;
		default:
			/* メッセージ行をパースする */
			if (!parse_message(cmd_size, fname, line, buf))
				result = false;
			else
				cmd_size++;
			break;
		}
		line++;
	}		

	close_rfile(rf);
	return result;
}

/* 命令行をパースする */
static bool parse_insn(int index, const char *file, int line, const char *buf)
{
	struct command *c;
	char *tp;
	int i, min = 0, max = 0;

	c = &cmd[index];

	/* 行番号とオリジナルの行を保存しておく */
	c->line = line;
	c->text = strdup(buf);
	if (c->text == NULL) {
		log_memory();
		return false;
	}

	/* トークン化する文字列を複製する */
	c->param[0] = strdup(buf);
	if (c->param[0] == NULL) {
		log_memory();
		return false;
	}

	/* 最初のトークンを切り出す */
	strtok(c->param[0], " ");

	/* コマンドのタイプを取得する */
	for (i = 0; i < (int)INSN_TBL_SIZE; i++) {
		if (strcmp(c->param[0], insn_tbl[i].str) == 0) {
			c->type = insn_tbl[i].type;
			min = insn_tbl[i].min;
			max = insn_tbl[i].max;
			break;
		}
	}
	if (i == INSN_TBL_SIZE) {
		log_script_command_not_found(c->param[0]);
		log_script_parse_footer(file, line, buf);
		return false;
	}

	/* 2番目以降のトークンを取得する */
	i = 1;
	while ((tp = strtok(NULL, " "))  != NULL && i < PARAM_SIZE) {
		c->param[i] = tp;
		i++;
	}

	/* パラメータの数をチェックする */
	if (i - 1 < min) {
		log_script_too_few_param(min, i - 1);
		log_script_parse_footer(file, line, buf);
		return false;
	}
	if (i - 1 > max) {
		log_script_too_many_param(max, i - 1);
		log_script_parse_footer(file, line, buf);
		return false;
	}

	return true;
}

/* セリフ行をパースする */
static bool parse_serif(int index, const char *file, int line, const char *buf)
{
	char *first, *second, *third;

	assert(buf[0] == '*');

	/* 行番号とオリジナルの行を保存しておく */
	cmd[index].type = COMMAND_SERIF;
	cmd[index].line = line;
	cmd[index].text = strdup(buf);
	if (cmd[index].text == NULL) {
		log_memory();
		return false;
	}

	/* トークン化する文字列を複製する */
	cmd[index].param[SERIF_PARAM_PTR] = strdup(&buf[1]);
	if (cmd[index].param[0] == NULL) {
		log_memory();
		return false;
	}

	/* トークンを取得する(2つか3つある) */
	first = strtok(cmd[index].param[0], "*");
	second = strtok(NULL, "*");
	third = strtok(NULL, "*");
	if (first == NULL || second == NULL) {
		log_script_empty_serif();
		log_script_parse_footer(file, line, buf);
		return false;
	}

	/* トークンの数で場合分けする */
	if (third != NULL) {
		cmd[index].param[SERIF_PARAM_NAME] = first;
		cmd[index].param[SERIF_PARAM_VOICE] = second;
		cmd[index].param[SERIF_PARAM_MESSAGE] = third;
	} else {
		cmd[index].param[SERIF_PARAM_NAME] = first;
		cmd[index].param[SERIF_PARAM_VOICE] = NULL;
		cmd[index].param[SERIF_PARAM_MESSAGE] = second;
	}

	/* 成功 */
	return true;
}

/* メッセージ行をパースする */
static bool parse_message(int index, const char *file, int line,
			  const char *buf)
{
	UNUSED_PARAMETER(file);

	/* 行番号とオリジナルの行(メッセージ全体)を保存しておく */
	cmd[index].type = COMMAND_MESSAGE;
	cmd[index].line = line;
	cmd[index].text = strdup(buf);
	if (cmd[index].text == NULL) {
		log_memory();
		return false;
	}

	/* 成功 */
	return true;
}

/* ラベル行をパースする */
static bool parse_label(int index, const char *file, int line, const char *buf)
{
	UNUSED_PARAMETER(file);

	/* 行番号とオリジナルの行(メッセージ全体)を保存しておく */
	cmd[index].type = COMMAND_LABEL;
	cmd[index].line = line;
	cmd[index].text = strdup(buf);
	if (cmd[index].text == NULL) {
		log_memory();
		return false;
	}

	/* ラベルを保存する */
	cmd[index].param[LABEL_PARAM_LABEL] = strdup(&buf[1]);
	if (cmd[index].param[LABEL_PARAM_LABEL] == NULL) {
		log_memory();
		return false;
	}

	/* 成功 */
	return true;
}
