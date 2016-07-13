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

/*
 * 変数を文字列で指定して取得する
 */
bool get_variable_by_string(const char *var, int32_t *val)
{
	int index;

	if (var[0] != '$' || strlen(var) == 1) {
		log_script_not_variable(var);
		log_script_exec_footer();
		return false;
	}

	index = atoi(&var[1]);
	if (index < 0 || index >= VAR_SIZE) {
		log_script_var_index(index);
		log_script_exec_footer();
		return false;
	}

	*val = var_tbl[index];
	return true;
}

/*
 * 変数を文字列で指定して設定する
 */
bool set_variable_by_string(const char *var, int32_t val)
{
	int index;

	if (var[0] != '$' || strlen(var) == 1) {
		log_script_not_variable(var);
		log_script_exec_footer();
		return false;
	}

	index = atoi(&var[1]);
	if (index < 0 || index >= VAR_SIZE) {
		log_script_var_index(index);
		log_script_exec_footer();
		return false;
	}

	var_tbl[index] = val;
	return true;
}
