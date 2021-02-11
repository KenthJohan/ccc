/* SPDX-License-Identifier: GPL-2.0 */
#pragma once
#include <stddef.h> //offsetof
#include <stdint.h> //offsetof
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "../csc_basic.h"
#include "../csc_debug.h"



#define CSC_GLPOINTCLOUD_POS_DIM 4
#define CSC_GLPOINTCLOUD_COL_DIM 4


struct csc_glpointcloud
{
	uint32_t cap;
	GLuint vbop;
	GLuint vboc;
	GLuint vao;
	float * vpos;
	uint32_t * vcol;
	GLint glprogram;
	GLint uniform_mvp;
};


void csc_glpointcloud_init (struct csc_glpointcloud * item)
{
	ASSERT (item->glprogram > 0);
	item->uniform_mvp = glGetUniformLocation (item->glprogram, "mvp");
	ASSERT (item->uniform_mvp >= 0);

	item->vpos = calloc (item->cap * CSC_GLPOINTCLOUD_POS_DIM, sizeof(float));
	item->vcol = calloc (item->cap * CSC_GLPOINTCLOUD_COL_DIM, sizeof(uint32_t));

	float * p = item->vpos;
	for (uint32_t i = 0; i < item->cap; ++i)
	{
		p[0] = (((float)rand() / (float)RAND_MAX)-0.5f) * 1.0f;
		p[1] = (((float)rand() / (float)RAND_MAX)-0.5f) * 1.0f;
		p[2] = (((float)rand() / (float)RAND_MAX)-0.5f) * 1.0f;
		p[3] = 1.0f;
		item->vcol[i] = 0xFFFFAAFF;
		p += CSC_GLPOINTCLOUD_POS_DIM;
	}


	glGenVertexArrays (1, &item->vao);
	glGenBuffers (1, &item->vbop);
	glGenBuffers (1, &item->vboc);

	glBindVertexArray (item->vao);

	glBindBuffer (GL_ARRAY_BUFFER, item->vbop);
	glBufferData (GL_ARRAY_BUFFER, item->cap * CSC_GLPOINTCLOUD_POS_DIM * sizeof(float), item->vpos, GL_DYNAMIC_DRAW);
	glVertexAttribPointer (0, CSC_GLPOINTCLOUD_POS_DIM, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray (0);

	glBindBuffer (GL_ARRAY_BUFFER, item->vboc);
	glBufferData (GL_ARRAY_BUFFER, item->cap * CSC_GLPOINTCLOUD_COL_DIM * sizeof(uint32_t), item->vcol, GL_DYNAMIC_DRAW);
	glVertexAttribPointer (1, CSC_GLPOINTCLOUD_COL_DIM, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);
	glEnableVertexAttribArray (1);
}


void csc_glpointcloud_update (struct csc_glpointcloud * item, uint32_t from, uint32_t count, void * data)
{
	glBindBuffer (GL_ARRAY_BUFFER, item->vbop);
	GLintptr offset = from * CSC_GLPOINTCLOUD_POS_DIM * sizeof (float);
	GLsizeiptr size = count * CSC_GLPOINTCLOUD_POS_DIM * sizeof (float);
	glBufferSubData (GL_ARRAY_BUFFER, offset, size, data);
}



void csc_glpointcloud_draw (struct csc_glpointcloud * item, float mvp[4*4])
{
	glBindVertexArray (item->vao);
	glUseProgram (item->glprogram);
	glUniformMatrix4fv (item->uniform_mvp, 1, GL_FALSE, (const GLfloat *) mvp);
	glDrawArrays (GL_POINTS, 0, item->cap);
}




static void csc_glpointcloud_begin_draw (struct csc_glpointcloud * item, float mvp[4*4])
{
	glBindVertexArray (item->vao);
	glUseProgram (item->glprogram);
	glUniformMatrix4fv (item->uniform_mvp, 1, GL_FALSE, (const GLfloat *) mvp);
}


















