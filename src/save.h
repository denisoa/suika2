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

#ifndef SUIKA_SAVE_H
#define SUIKA_SAVE_H

#include "types.h"

/* セーブデータに関する初期化処理を行う */
bool init_save(void);

/* セーブデータに関する終了処理を行う */
void cleanup_save(void);

/* コマンドがロードによって開始されたかを確認する */
bool check_load_flag(void);

/* コマンドがセーブ画面から復帰したかを確認する */
bool check_restore_flag(void);

/* セーブ画面を開始する */
void start_save_mode(bool load_only);

/* セーブ画面が有効であるかを返す */
bool is_save_mode(void);

/* セーブ画面を実行する */
void run_save_mode(int *x, int *y, int *w, int *h);

#endif
