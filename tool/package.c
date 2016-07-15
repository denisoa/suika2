/* -*- coding: utf-8-with-signature; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika 2
 * Copyright (C) 2001-2016, TABATA Keiichi. All rights reserved.
 */

/*
 * パッケージャ
 *
 * [Changes]
 *  - 2016/07/14 作成
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ファイルエントリの最大数 */
#define ENTRY_SIZE		(10000)

/* ファイル名のサイズ */
#define FILE_NAME_SIZE		(256)

/* アーカイブ先頭に書き出すファイル数のバイト数 */
#define FILE_COUNT_BYTES	(8)

/* ファイルエントリのサイズ */
#define ENTRY_BYTES		(256 + 8 + 8)

/* パッケージファイル名 */
#define PACKAGE_FILE_NAME	"data01.arc"

/* ディレクトリ名 */
const char *dir_names[] = {
	"bg", "bgm", "ch", "cg", "cv", "conf", "font", "se", "txt"
};

/* ディレクトリ名の数 */
#define DIR_COUNT	(sizeof(dir_names) / sizeof(const char *))

/* ファイルエントリ */
struct entry {
	/* ファイル名 */
	char file_name[FILE_NAME_SIZE];

	/* ファイルサイズ */
	uint64_t file_size;

	/* ファイルのアーカイブ内でのオフセット */
	uint64_t file_offset;
} entry[ENTRY_SIZE];

/* ファイル数 */
uint64_t file_count;

/* 現在処理中のファイルのアーカイブ内でのオフセット */
uint64_t offset;

/* 前方参照 */
bool write_file_entries(FILE *fp);
bool write_file_bodies(FILE *fp);

#ifdef _WIN32

#include <windows.h>

/*
 * ディレクトリのファイル一覧を取得する(Windows版)
 */
bool get_file_names(const char *dir)
{
    char path[256];
    HANDLE hFind;
    WIN32_FIND_DATA wfd;

    snprintf(path, sizeof(path), "%s\\*.*", dir);

    hFind = FindFirstFile(path, &wfd);
    if(hFind == INVALID_HANDLE_VALUE)
    {
        printf("Directory %s not found.\n", dir);
        return false;
    }

    do
    {
        if(!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            snprintf(entry[file_count].file_name, FILE_NAME_SIZE,
                     "%s/%s", dir, wfd.cFileName);
            printf("  %s\n", entry[file_count].file_name);
            file_count++;
	}
    } while(FindNextFile(hFind, &wfd));

    FindClose(hFind);
    return true;
}

#else

#include <dirent.h>

/*
 * ディレクトリのファイル一覧を取得する(UNIX版)
 */
bool get_file_names(const char *dir)
{
	char path[FILE_NAME_SIZE];
	struct dirent **names;
	int i, count;

	/* ディレクトリの内容を取得する */
	count = scandir(dir, &names, NULL, alphasort);
	if (count < 0) {
		printf("Directory %s not found.\n", dir);
		return false;
	}
	if (count > ENTRY_SIZE) {
		printf("Too many files.");
		return false;	/* Intentional memory leak. */
	}
	for (i = 0; i < count; i++) {
		if (names[i]->d_name[0] == '.')
			continue;

		snprintf(entry[file_count].file_name, FILE_NAME_SIZE,
			 "%s/%s", dir, names[i]->d_name);

		printf("  %s\n", entry[file_count].file_name);
		free(names[i]);
		file_count++;
	}
	free(names);

	/* オフセットの値を計算する */
	offset = FILE_COUNT_BYTES + ENTRY_BYTES * file_count;
	return true;
}

#endif

/*
 * 各ファイルのサイズを求める
 */
bool get_file_sizes(void)
{
	uint64_t i;
	FILE *fp;

	/* 各ファイルのサイズを求め、オフセットを計算する */
	for (i = 0; i < file_count; i++) {
#ifdef _WIN32
		char *path = strdup(entry[i].file_name);
		*strchr(path, '/') = '\\';
		fp = fopen(path, "rb");
#else
		fp = fopen(entry[i].file_name, "r");
#endif
		if (fp == NULL) {
			printf("Can't open file %s\n", entry[i].file_name);
			return false;
		}
		fseek(fp, 0, SEEK_END);
		entry[i].file_size = ftell(fp);
		entry[i].file_offset = offset;
		fclose(fp);
#ifdef _WIN32
		free(path);
#endif
		offset += entry[i].file_size;
	}
	return true;
}

/* アーカイブファイルを書き出す */
bool write_archive_file(void)
{
	FILE *fp;
	bool success;

	fp = fopen(PACKAGE_FILE_NAME, "wb");
	if (fp == NULL) {
		printf("Can't open %s\n", PACKAGE_FILE_NAME);
		return false;
	}

	success = false;
	do {
		if (fwrite(&file_count, sizeof(uint64_t), 1, fp) < 1)
			break;
		if (!write_file_entries(fp))
			break;
		if (!write_file_bodies(fp))
			break;
	} while (0);

	return true;
}

/* ファイルのエントリを出力する */
bool write_file_entries(FILE *fp)
{
	uint64_t i;

	for (i = 0; i < file_count; i++) {
		if (fwrite(&entry[i].file_name, FILE_NAME_SIZE, 1, fp) < 1)
			return false;
		if (fwrite(&entry[i].file_size, sizeof(uint64_t), 1, fp) < 1)
			return false;
		if (fwrite(&entry[i].file_offset, sizeof(uint64_t), 1, fp) < 1)
			return false;
	}
	return true;
}

/* ファイルのエントリを出力する */
bool write_file_bodies(FILE *fp)
{
	char buf[8192];
	FILE *fpin;
	uint64_t i;
	size_t len;

	for (i = 0; i < file_count; i++) {
#ifdef _WIN32
		char *path = strdup(entry[i].file_name);
		*strchr(path, '/') = '\\';
		fpin = fopen(path, "rb");
#else
		fpin = fopen(entry[i].file_name, "r");
#endif
		if (fpin == NULL) {
			printf("Can't open %s\n", entry[i].file_name);
			return false;
		}

		do  {
			len = fread(buf, 1, sizeof(buf), fpin);
			if (len > 0)
				if (fwrite(buf, len, 1, fp) < 1)
					return false;
		} while (len == sizeof(buf));
#ifdef _WIN32
		free(path);
#endif
	}
	return true;
}

int main(int argc, char *argv[])
{
	int i;

	printf("Hello, this is Suika 2's packager.\n");

	/* ファイルの一覧を取得する */
	printf("Searching files...\n");
	for (i = 0; i < DIR_COUNT; i++)
		if (!get_file_names(dir_names[i]))
			return 1;

	/* ファイルのサイズを取得し、アーカイブ内でのオフセットを決定する */
	printf("Checking file sizes...\n");
	if (!get_file_sizes())
		return 1;

	/* アーカイブファイルを書き出す */
	printf("Writing archive files...\n");
	if (!write_archive_file())
		return 1;

	printf("Done.\n");
	return 0;
}
