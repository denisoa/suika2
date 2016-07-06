/* -*- coding: utf-8-with-signature; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika 2
 * Copyright (C) 2001-2016, TABATA Keiichi. All rights reserved.
 */

/*
 * [Changes]
 *  - 2016/05/27 作成
 */

#ifndef SUIKA_SUIKA_H
#define SUIKA_SUIKA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#include "conf.h"
#include "event.h"
#include "file.h"
#include "glyph.h"
#include "image.h"
#include "log.h"
#include "main.h"
#include "mixer.h"
#include "platform.h"
#include "script.h"
#include "save.h"
#include "stage.h"
#include "vars.h"
#include "wave.h"

/*
 * ミキサのストリーム
 */
#define MIXER_STREAMS	(3)
#define BGM_STREAM	(0)
#define VOICE_STREAM	(1)
#define SE_STREAM	(2)

/*
 * 固定のディレクトリ名
 */

/* 背景イメージのディレクトリ */
#define BG_DIR		"bg"

/* キャライメージのディレクトリ */
#define CH_DIR		"ch"

/* BGMのディレクトリ */
#define BGM_DIR		"bgm"

/* SEのディレクトリ */
#define SE_DIR		"se"

/* ボイスのディレクトリ */
#define CV_DIR		"cv"

/* その他のCGのディレクトリ */
#define CG_DIR		"cg"

/* スクリプトのディレクトリ */
#define SCRIPT_DIR	"txt"

/* フォントのディレクトリ */
#define FONT_DIR	"font"

/* 設定ファイルのディレクトリ */
#define CONF_DIR	"conf"

/* セーブデータのディレクトリ */
#define SAVE_DIR	"sav"

/*
 * 固定のファイル名
 */

/* ログファイル名 */
#define LOG_FILE	"log.txt"

/* 設定ファイル名 */
#define PROP_FILE	"config.txt"

/* 初期スクリプト */
#define INIT_FILE	"init.txt"

#endif
