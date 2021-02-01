/* SPDX-License-Identifier: GPL-2.0 */
#pragma once
#include <stddef.h> //offsetof
#include <stdint.h> //offsetof
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "csc_basic.h"
#include "csc_debug.h"





struct csc_rectangle_v4f32
{
	float a[4];
	float b[4];
	float c[4];
	float d[4];
	float e[4];
	float f[4];
};


void csc_rectangle_setcoord_v4f32 (struct csc_rectangle_v4f32 * item, float x, float y, float z, float w, float width, float height)
{
	float wr = width / 2.0f;
	float hr = height / 2.0f;

	item->a[0] = -wr + x;
	item->a[1] = -hr + y;
	item->a[2] = z;
	item->a[3] = w;

	item->b[0] = wr + x;
	item->b[1] = -hr + y;
	item->b[2] = z;
	item->b[3] = w;

	item->c[0] = wr + x;
	item->c[1] = hr + y;
	item->c[2] = z;
	item->c[3] = w;

	item->d[0] = -wr + x;
	item->d[1] = -hr + y;
	item->d[2] = z;
	item->d[3] = w;

	item->e[0] = wr + x;
	item->e[1] = hr + y;
	item->e[2] = z;
	item->e[3] = w;

	item->f[0] = -wr + x;
	item->f[1] = hr + y;
	item->f[2] = z;
	item->f[3] = w;
}




void csc_rectangle_setuv_vf32 (float uv[], uint32_t n)
{
	for (uint32_t i = 0; i < n; ++i)
	{
		uv[0] = 0.0f;
		uv[1] = 0.0f;

		uv[2] = 1.0f;
		uv[3] = 0.0f;

		uv[4] = 1.0f;
		uv[5] = 1.0f;

		uv[6] = 0.0f;
		uv[7] = 0.0f;

		uv[8] = 1.0f;
		uv[9] = 1.0f;

		uv[10] = 0.0f;
		uv[11] = 1.0f;
		uv += 12;
	}
}


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
	csc_rectangle_setuv_vf32 (img->vtex, img->cap);
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
		csc_rectangle_setcoord_v4f32((void*)vpos, img[i].x, img[i].y, img[i].z, img[i].layer, img[i].width, img[i].height);
		vpos += CSC_GLIMAGE_POS_DIM*CSC_GLIMAGE_VERTS_COUNT;
	}
	glBindBuffer (GL_ARRAY_BUFFER, ctx->vbop);
	glBufferSubData (GL_ARRAY_BUFFER, 0, ctx->cap * CSC_GLIMAGE_VERTS_COUNT * CSC_GLIMAGE_POS_DIM * sizeof (float), ctx->vpos);
}






