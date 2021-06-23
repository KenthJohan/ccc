/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once
#include <GL/glew.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csc_basic.h"
#include "csc_debug.h"
#include "csc_malloc_file.h"


char * csc_gl_infolog_malloc (GLuint shader)
{
	GLint length = 0;
	glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &length);
	char * info = malloc (sizeof(char) * length);
	glGetShaderInfoLog (shader, length, NULL, info);
	return info;
}


void csc_gl_infolog_fput (GLuint shader, FILE * f)
{
	char * info = csc_gl_infolog_malloc (shader);
	fputs (info, f);
	free (info);
}


GLint csc_gl_shader_from_file (char const * filename)
{
	ASSERT (filename);
	GLuint shader;
	char const * ext = strrchr (filename, '.');
	if (ext == NULL)
	{
		return -1;
	}
	ext++;
	if (strlen (ext) != 4)
	{
		return -1;
	}
	switch (STR4_U32_LE (ext))
	{
	case U8_U32_LE ('g', 'l', 'v', 's'):
		shader = glCreateShader (GL_VERTEX_SHADER);
		break;
	case U8_U32_LE ('g', 'l', 'f', 's'):
		shader = glCreateShader (GL_FRAGMENT_SHADER);
		break;
	default:
		return -1;
	}
	char const * source = csc_malloc_file (filename);
	glShaderSource (shader, 1, (const GLchar **)&source, NULL);
	glCompileShader (shader);
	GLint status;
	glGetShaderiv (shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		fprintf (stderr, "vertex shader compilation failed\n" );
		csc_gl_infolog_fput (shader, stderr);
		return -1;
	}
	return shader;
}


GLint csc_gl_program_from_files (char const * filenames[])
{
	GLint program = glCreateProgram();
	while (filenames[0])
	{
		if (filenames[0] == NULL) {break;}
		GLint shader;
		shader = csc_gl_shader_from_file (filenames[0]);
		glAttachShader (program, shader);
		filenames++;
	}
	return program;
}



#define CSC_GL_PROGRAM_FROM_FILES1_MAXFILENAME 128
GLint csc_gl_program_from_files1 (char const * filenames)
{
	char buffer[CSC_GL_PROGRAM_FROM_FILES1_MAXFILENAME];
	GLint program = glCreateProgram();
	GLint shader;
	while (1)
	{
		char * e = strchr (filenames, ';');
		if (e)
		{
			int l = e - filenames;
			memcpy (buffer, filenames, l);
			buffer[l] = '\0';
			shader = csc_gl_shader_from_file (buffer);
			glAttachShader (program, shader);
			filenames = e + 1;
		}
		else
		{
			shader = csc_gl_shader_from_file (filenames);
			glAttachShader (program, shader);
			break;
		}
	}
	GLint logLength;
	GLint status;
	glLinkProgram (program);


	glGetProgramiv (program, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		GLchar *log = (GLchar *)malloc(logLength);
		glGetProgramInfoLog(program, logLength, &logLength, log);
		printf( "Program link log: %s", log );
		free(log);
	}

	glGetShaderiv (shader, GL_COMPILE_STATUS, &status);
	if (status == 0 )
	{
		return 0;
	}
	return program;
}










void csc_gl_make_rectangle_pos (float v[], float x, float y, float z, float w, float width, float height, uint32_t stride)
{
	float wr = width / 2.0f;
	float hr = height / 2.0f;

	v[0] = -wr + x;
	v[1] = -hr + y;
	v[2] = z;
	v[3] = w;
	v += stride;

	v[0] = wr + x;
	v[1] = -hr + y;
	v[2] = z;
	v[3] = w;
	v += stride;

	v[0] = wr + x;
	v[1] = hr + y;
	v[2] = z;
	v[3] = w;
	v += stride;

	v[0] = -wr + x;
	v[1] = -hr + y;
	v[2] = z;
	v[3] = w;
	v += stride;

	v[0] = wr + x;
	v[1] = hr + y;
	v[2] = z;
	v[3] = w;
	v += stride;

	v[0] = -wr + x;
	v[1] = hr + y;
	v[2] = z;
	v[3] = w;
	v += stride;
}


void csc_gl_make_rectangle_uv (float uv[], uint32_t n, uint32_t stride)
{
	for (uint32_t i = 0; i < n; ++i)
	{
		uv[0] = 0.0f;
		uv[1] = 0.0f;
		uv += stride;

		uv[0] = 1.0f;
		uv[1] = 0.0f;
		uv += stride;

		uv[0] = 1.0f;
		uv[1] = 1.0f;
		uv += stride;

		uv[0] = 0.0f;
		uv[1] = 0.0f;
		uv += stride;

		uv[0] = 1.0f;
		uv[1] = 1.0f;
		uv += stride;

		uv[0] = 0.0f;
		uv[1] = 1.0f;
		uv += stride;
	}
}




