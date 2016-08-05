/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika 2
 * Copyright (C) 2001-2016, TABATA Keiichi. All rights reserved.
 */

/*
 * NDKメインモジュール
 */

#include "ndkmain.h"
#include "image.h"

/*
 * MainActivityのインスタンスへの参照 
 */
jobject main_activity;

/*
 * JNI関数呼び出しの間だけ有効なJNIEnvへの参照
 */
JNIEnv *jni_env;

/*
 * 背景イメージ
 */
static struct image *back_image;

/*
 * 初期化処理を行います。
 */
JNIEXPORT jobject JNICALL
Java_jp_luxion_suika_MainActivity_init(
	JNIEnv *env,
	jobject instance)
{
	jobject back_bitmap;

	/* アクティビティを保持する */
	main_activity = (*env)->NewGlobalRef(env, instance);

	/* この関数呼び出しの間だけenvをグローバル変数で参照する */
	jni_env = env;

	/* 背景イメージを作成する */
	back_image = create_image_from_file("bg", "roof.png");
	back_bitmap = (jobject)get_image_system_object(back_image);
	/* 
         * jclass cls = (*env)->FindClass(env, "jp/luxion/suika/MainActivity");
	 * jmethodID mid = (*env)->GetMethodID(env, cls, "loadBitmap", "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
	 * backImage = (*env)->CallObjectMethod(env, instance, mid, (*env)->NewStringUTF(env, "bg/roof.png"));
         */

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
	/* この関数呼び出しの間だけenvをグローバル変数で参照する */
	jni_env = env;

	/* envをグローバル変数で参照するのを終了する */
	jni_env = NULL;

	return JNI_TRUE;
}
