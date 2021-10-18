/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once
#include "csc_assert.h"
#include "csc_basic.h"
#include <stdio.h> //fopen
#include <string.h> //memset
#include <stdlib.h> //malloc

static char * csc_malloc_file (char const * filename)
{
	ASSERTF (filename != NULL, "filename is NULL%s", "");
	FILE * file = fopen (filename, "rb");
	ASSERTF (file != NULL, "can not open file '%s'", filename);
	int r;
	r = fseek (file, 0, SEEK_END);
	ASSERT (r == 0);
	long length = ftell (file);
	ASSERT (length >= 0);
	r = fseek (file, 0, SEEK_SET);
	ASSERT (r == 0);
	char * buffer = (char *) malloc ((unsigned) length + 1);
	ASSERTF (buffer != NULL, "buffer is NULL%s", "");
	memset (buffer, 0, (unsigned) length + 1);
	//buffer [length + 1] = 0;
	if (length > 0)
	{
		size_t n = fread (buffer, (unsigned) length, 1, file);
		ASSERTF (n == 1, "fread error %i %i", (int)n, (int)length);
	}
	fclose (file);
	return buffer;
}


static char * csc_malloc_file_ab (char const * a, char const * b)
{
	char filename[100];
	memset (filename, 0, 100);
	memcpy (filename, a, MIN ((size_t)(b-a), sizeof (filename)-1));
	FILE * file = fopen (filename, "rb");
	//ASSERTF (file != NULL, "can not open file '%s'", filename);
	if (file == NULL)
	{
		return NULL;
	}
	int r;
	r = fseek (file, 0, SEEK_END);
	ASSERT (r == 0);
	long length = ftell (file);
	ASSERT (length >= 0);
	r = fseek (file, 0, SEEK_SET);
	ASSERT (r == 0);
	char * buffer = (char *) malloc ((unsigned) length + 1);
	ASSERTF (buffer != NULL, "buffer is NULL%s", "");
	memset (buffer, 0, (unsigned) length + 1);
	//buffer [length + 1] = 0;
	if (length > 0)
	{
		size_t n = fread (buffer, (unsigned) length, 1, file);
		ASSERTF (n == 1, "fread error %i %i", (int)n, (int)length);
	}
	fclose (file);
	return buffer;
}


static char * csc_malloc_file1 (char const * filename, long * length)
{
	ASSERTF (filename != NULL, "filename is NULL%s", "");
	FILE * file = fopen (filename, "rb");
	ASSERTF (file != NULL, "can not open file '%s'", filename);
	int r;
	r = fseek (file, 0, SEEK_END);
	ASSERT (r == 0);
	*length = ftell (file);
	ASSERT ((*length) >= 0);
	r = fseek (file, 0, SEEK_SET);
	ASSERT (r == 0);
	char * buffer = (char *) malloc ((unsigned) (*length) + 1);
	ASSERTF (buffer != NULL, "buffer is NULL%s", "");
	memset (buffer, 0, (unsigned) (*length) + 1);
	//buffer [length + 1] = 0;
	if (*length > 0)
	{
		size_t n = fread (buffer, (unsigned) (*length), 1, file);
		ASSERTF (n == 1, "fread error %i %i", (int)n, (int)(*length));
	}
	fclose (file);
	return buffer;
}
