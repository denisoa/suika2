/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika 2
 * Copyright (C) 2001-2016, TABATA Keiichi. All rights reserved.
 */

/*
 * NDKメインモジュール
 */

#include "suika.h"
#include "ndkmain.h"

/*
 * JNI関数呼び出しの間だけ有効なJNIEnvへの参照
 */
JNIEnv *jni_env;

/*
 * MainActivityのインスタンスへの参照 
 */
jobject main_activity;

/*
 * 背景イメージ
 */
static struct image *back_image;

static struct image *fore_image;

/*
 * 初期化処理を行います。
 */
JNIEXPORT jobject JNICALL
Java_jp_luxion_suika_MainActivity_init(
	JNIEnv *env,
	jobject instance,
	jobject assetManager)
{
	jobject back_bitmap;

	/* Activityを保持する */
	main_activity = (*env)->NewGlobalRef(env, instance);

	/* この関数呼び出しの間だけenvをグローバル変数で参照する */
	jni_env = env;

	/* 背景イメージを作成する */
	back_image = create_image(1280, 720);
	back_bitmap = (jobject)get_image_object(back_image);

	/* テスト用のイメージを作成する */
	fore_image = create_image_from_file("ch", "001-fun.png");

	/* envをグローバル変数で参照するのを終了する */
	jni_env = NULL;

	return back_bitmap;
}

/*
 * 終了処理を行います。
 */
JNIEXPORT void JNICALL
Java_jp_luxion_suika_MainActivity_cleanup(
	JNIEnv *env,
	jobject instance)
{
	/* envをグローバル変数で参照する */
	jni_env = env;

	if (back_image != NULL) {
		destroy_image(back_image);
		back_image = NULL;
	}
	if (main_activity != NULL) {
		(*env)->DeleteGlobalRef(env, main_activity);
		main_activity = NULL;
	}

	/* envをグローバル変数で参照するのを終了する */
	jni_env = NULL;
}

/*
 * フレーム処理を行います。
 */
JNIEXPORT jboolean JNICALL
Java_jp_luxion_suika_MainActivity_frame(
	JNIEnv *env,
	jobject instance)
{
	static int alpha = 0;

	/* この関数呼び出しの間だけenvをグローバル変数で参照する */
	jni_env = env;

	clear_image_color(back_image, 0xff0000ff);

	draw_image(back_image, 0, 0, fore_image, get_image_width(fore_image),
		   get_image_height(fore_image), 0, 0, alpha, BLEND_NORMAL);

	alpha = (alpha + 1) % 256;

	/* envをグローバル変数で参照するのを終了する */
	jni_env = NULL;

	return JNI_TRUE;
}

/*
 * platform.hの実装
 */

/*
 * infoログを出力する
 */
bool log_info(const char *s, ...)
{
	return true;
}

/*
 * warnログを出力する
 */
bool log_warn(const char *s, ...)
{
	return true;
}

/*
 * errorログを出力する
 */
bool log_error(const char *s, ...)
{
	return true;
}

/*
 * セーブディレクトリを作成する
 */
bool make_sav_dir(void)
{
	return true;
}

/*
 * データのディレクトリ名とファイル名を指定して有効なパスを取得する
 */
char *make_valid_path(const char *dir, const char *fname)
{
	/* NDKでは使用しない */
	assert(0);
	return NULL;
}

/*
 * バックイメージを取得する
 */
struct image *get_back_image(void)
{
	return back_image;
}

/*
 * タイマをリセットする
 */
void reset_stop_watch(stop_watch_t *t)
{
	jclass cls;
	jmethodID mid;
	long ret;

	/* 現在の時刻を取得する */
	cls = (*jni_env)->FindClass(jni_env, "java/lang/System");
	mid = (*jni_env)->GetMethodID(jni_env, cls, "currentTimeMillis", "()J");
	ret = (*jni_env)->CallStaticLongMethod(jni_env, cls, mid);

	/* 時刻を格納する */
	*t = (stop_watch_t)ret;
}

/*
 * タイマのラップをミリ秒単位で取得する
 */
int get_stop_watch_lap(stop_watch_t *t)
{
	jclass cls;
	jmethodID mid;
	long ret;

	/* 現在の時刻を取得する */
	cls = (*jni_env)->FindClass(jni_env, "java/lang/System");
	mid = (*jni_env)->GetMethodID(jni_env, cls, "currentTimeMillis", "()J");
	ret = (*jni_env)->CallStaticLongMethod(jni_env, cls, mid);

	return (int)(ret - (long)t);
}

/*
 * サウンドを再生を開始する
 */
bool play_sound(int n, struct wave *w)
{
	return true;
}

/*
 * サウンドの再生を停止する
 */
bool stop_sound(int n)
{
	return true;
}

/*
 * サウンドのボリュームを設定する
 */
bool set_sound_volume(int n, float vol)
{
	return true;
}
