 /* SPDX-License-Identifier: GPL-2.0 */
#pragma once
#include <stddef.h> //offsetof
#include <stdint.h> //offsetof
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "csc_basic.h"
#include "csc_debug.h"
#include "csc_gl.h"





#define CSC_GLIMAGE_POS_DIM 4
#define CSC_GLIMAGE_TEX_DIM 2
#define CSC_GLIMAGE_VERTS_COUNT 6


struct csc_glimgcontext
{
	uint32_t cap;
	GLuint vbop;
	GLuint vbot;
	GLuint vao;
	float * vpos;
	float * vtex;

	GLint glprogram;
	GLint uniform_mvp;
	GLint uniform_texture1;
};


void csc_glimage_init (struct csc_glimgcontext * img)
{
	img->uniform_mvp = glGetUniformLocation (img->glprogram, "mvp");
	img->uniform_texture1 = glGetUniformLocation (img->glprogram, "texture1");
	ASSERT (img->uniform_mvp >= 0);
	ASSERT (img->uniform_texture1 >= 0);

	img->vpos = calloc (img->cap * CSC_GLIMAGE_VERTS_COUNT * CSC_GLIMAGE_POS_DIM, sizeof(float));
	img->vtex = calloc (img->cap * CSC_GLIMAGE_VERTS_COUNT * CSC_GLIMAGE_TEX_DIM, sizeof(float));

	glGenVertexArrays (1, &img->vao);
	glGenBuffers (1, &img->vbop);
	glGenBuffers (1, &img->vbot);

	glBindVertexArray (img->vao);

	glBindBuffer (GL_ARRAY_BUFFER, img->vbop);
	glBufferData (GL_ARRAY_BUFFER, img->cap * CSC_GLIMAGE_VERTS_COUNT * CSC_GLIMAGE_POS_DIM * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer (0, CSC_GLIMAGE_POS_DIM, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray (0);

	glBindBuffer (GL_ARRAY_BUFFER, img->vbot);
	csc_gl_make_rectangle_uv (img->vtex, img->cap, 2);
	glBufferData (GL_ARRAY_BUFFER, img->cap * CSC_GLIMAGE_VERTS_COUNT * CSC_GLIMAGE_TEX_DIM * sizeof(float), img->vtex, GL_DYNAMIC_DRAW);
	glVertexAttribPointer (1, CSC_GLIMAGE_TEX_DIM, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray (1);
}


static void csc_glimage_begin_draw (struct csc_glimgcontext * ctx, float mvp[4*4])
{
	glBindVertexArray (ctx->vao);
	glUseProgram (ctx->glprogram);
	glUniform1i (ctx->uniform_texture1, 0);
	glUniformMatrix4fv (ctx->uniform_mvp, 1, GL_FALSE, (const GLfloat *) mvp);
}


static void csc_glimage_update (struct csc_glimgcontext * ctx, float v[], uint32_t n)
{
	glBindBuffer (GL_ARRAY_BUFFER, ctx->vbop);
	glBufferSubData (GL_ARRAY_BUFFER, 0, n * CSC_GLIMAGE_VERTS_COUNT * CSC_GLIMAGE_POS_DIM * sizeof (float), v);
}






