/* -*- coding: utf-8-with-signature; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika 2
 * Copyright (C) 2001-2016, TABATA Keiichi. All rights reserved.
 */

/*
 * [Changes]
 *  - 2016/06/28 作成
 */

#include "suika.h"

/*
 * ファイル読み込みストリーム
 *  - まだスタブ
 *  - アーカイブをサポートする
 */
struct rfile {
	FILE *fp;
};

/*
 * ファイル読み込みストリーム
 */
struct wfile {
	FILE *fp;
};

/*
 * 読み込み
 */

/*
 * ファイル読み込みストリームを開く
 */
struct rfile *open_rfile(const char *dir, const char *file, bool no_error)
{
	char *path;
	struct rfile *rf;

	/* rfile構造体のメモリを確保する */
	rf = malloc(sizeof(struct rfile));
	if (rf == NULL) {
		log_memory();
		return NULL;
	}

	/* パスを生成する */
	path = make_valid_path(dir, file);
	if (path == NULL) {
		log_memory();
		free(rf);
		return NULL;
	}

	/* ファイルをオープンする */
	rf->fp = fopen(path, "rb");
	if (rf->fp == NULL) {
		if (!no_error)
			log_file_open(path);
		free(path);
		free(rf);
		return NULL;
	}
	free(path);

	return rf;
}

/*
 * ファイル読み込みストリームから読み込む
 */
size_t read_rfile(struct rfile *rf, void *buf, size_t size)
{
	size_t len;

	assert(rf != NULL);
	assert(rf->fp != NULL);

	len = fread(buf, 1, size, rf->fp);

	return len;
}

/*
 * ファイル読み込みストリームから1行読み込む
 */
void *gets_rfile(struct rfile *rf, void *buf, size_t size)
{
	char *ptr;
	size_t len;
	int c;

	assert(rf != NULL);
	assert(rf->fp != NULL);

	ptr = (char *)buf;

	for (len = 0; len < size - 1; len++) {
		c = fgetc(rf->fp);
		if (c == EOF) {
			*ptr = '\0';
			return len == 0 ? NULL : buf;
		} else if (c == '\n' || c == '\0') {
			*ptr = '\0';
			return buf;
		} else if (c == '\r') {
			c = fgetc(rf->fp);
			if (c == '\n') {
				*ptr = '\0';
				return buf;
			} else {
				ungetc(c, rf->fp);
				*ptr = '\0';
				return buf;
			}
		} else {
			*ptr++ = (char)c;
		}
	}
	*ptr = '\0';
	return buf;
}

/*
 * ファイル読み込みストリームを閉じる
 */
void close_rfile(struct rfile *rf)
{
	assert(rf != NULL);
	assert(rf->fp != NULL);

	fclose(rf->fp);
	free(rf);
}

/*
 * 書き込み
 */

/*
 * ファイル書き込みストリームを開く
 */
struct wfile *open_wfile(const char *dir, const char *file)
{
	char *path;
	struct wfile *wf;

	/* wfile構造体のメモリを確保する */
	wf = malloc(sizeof(struct wfile));
	if (wf == NULL) {
		log_memory();
		return NULL;
	}

	/* パスを生成する */
	path = make_valid_path(dir, file);
	if (path == NULL) {
		log_memory();
		free(wf);
		return NULL;
	}

	/* ファイルをオープンする */
	wf->fp = fopen(path, "wb");
	if (wf->fp == NULL) {
		log_file_open(path);
		free(path);
		free(wf);
		return NULL;
	}
	free(path);

	return wf;
}

/*
 * ファイル書き込みストリームに書き込む
 */
size_t write_wfile(struct wfile *wf, const void *buf, size_t size)
{
	size_t len;

	assert(wf != NULL);
	assert(wf->fp != NULL);

	len = fwrite(buf, 1, size, wf->fp);

	return len;
}

/*
 * ファイル読み込みストリームを閉じる
 */
void close_wfile(struct wfile *wf)
{
	assert(wf != NULL);
	assert(wf->fp != NULL);

	fflush(wf->fp);
	fclose(wf->fp);
	free(wf);
}
