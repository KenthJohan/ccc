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



void csc_glimage_draw (struct csc_glimgcontext * img, float mvp[4*4])
{
	glBindVertexArray (img->vao);
	glUseProgram (img->glprogram);
	glUniform1i (img->uniform_texture1, 0);
	glUniformMatrix4fv (img->uniform_mvp, 1, GL_FALSE, (const GLfloat *) mvp);
	glDrawArrays (GL_TRIANGLES, 0, img->cap * CSC_GLIMAGE_VERTS_COUNT);
}


void csc_glimage_begin_draw(struct csc_glimgcontext * ctx, float mvp[4*4])
{
	glBindVertexArray (ctx->vao);
	glUseProgram (ctx->glprogram);
	glUniform1i (ctx->uniform_texture1, 0);
	glUniformMatrix4fv (ctx->uniform_mvp, 1, GL_FALSE, (const GLfloat *) mvp);
}
















struct csc_glimg
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		float pos[4];
	};
	float width;
	float height;
	uint32_t layer;
};


void csc_glimage_update (struct csc_glimgcontext * ctx, struct csc_glimg img[], uint32_t n)
{
	float * vpos = ctx->vpos;
	for (uint32_t i = 0; i < n; ++i)
	{
		csc_gl_make_rectangle_pos ((void*)vpos, img[i].x, img[i].y, img[i].z, img[i].layer, img[i].width, img[i].height, 4);
		vpos += CSC_GLIMAGE_POS_DIM*CSC_GLIMAGE_VERTS_COUNT;
	}
	glBindBuffer (GL_ARRAY_BUFFER, ctx->vbop);
	glBufferSubData (GL_ARRAY_BUFFER, 0, ctx->cap * CSC_GLIMAGE_VERTS_COUNT * CSC_GLIMAGE_POS_DIM * sizeof (float), ctx->vpos);
}






