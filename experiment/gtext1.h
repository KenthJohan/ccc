#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../csc_gft.h"
#include "../csc_math.h"
#include "../csc_vf32.h"



struct glx_vertex_manager
{
	GLuint vao;
	GLuint vbo;
	GLint program;
	GLint uniform_tex;
	uint32_t maxchars;
	uint32_t char_last;
	float * v;
	char * strbuf;
};


static void glx_vertex_manager_setup (struct glx_vertex_manager * ctx)
{
	ctx->char_last = 0;
	ctx->v = malloc (ctx->maxchars*4*6*sizeof(float));
	ctx->strbuf = malloc (ctx->maxchars);
	ASSERT_NOTNULL (ctx->v);
	ASSERT_NOTNULL (ctx->strbuf);
	ASSERT (glIsProgram(ctx->program));

	glUseProgram (ctx->program);
	ctx->uniform_tex = glGetUniformLocation (ctx->program, "tex0");
	ASSERT (ctx->uniform_tex >= 0);

	glGenBuffers (1, &ctx->vbo);
	glGenVertexArrays (1, &ctx->vao);
	glBindVertexArray (ctx->vao);
	glBindBuffer (GL_ARRAY_BUFFER, ctx->vbo);
	GLsizei stride = 2 * sizeof (float) + 3 * sizeof (float);
	GLintptr offset0 = 0;
	GLintptr offset1 = 2 * sizeof (float);
	glEnableVertexAttribArray (0);
	glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, stride, (void*)offset0);
	glEnableVertexAttribArray (1);
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset1);
}



static void glx_vertex_manager_drawtext
(struct glx_vertex_manager * ctx, struct gft_char c[128], struct gft_atlas * atlas, float x, float y, float sx, float sy, char const * text)
{
	uint32_t const stride = 5;
	ASSERT_PARAM_NOTNULL (ctx);
	ASSERT (glIsProgram(ctx->program));
	ASSERT (glIsVertexArray(ctx->vao));
	ASSERT (glIsBuffer(ctx->vbo));
	//ASSERT (strnlen (text, ctx->maxchars) <= ctx->maxchars);
	//memset (ctx->v, 0, sizeof (v));
	float * v = ctx->v;
	v += (ctx->char_last * stride * 6);
	uint32_t n;
	n = gft_gen_pos (v+0, ctx->maxchars, stride, text, c, x, y, sx, sy);
	n = gft_gen_uv  (v+2, ctx->maxchars, stride, text, c, atlas->w, atlas->h);
	ctx->char_last += n;
}


static void glx_vertex_manager_drawtextf
(struct glx_vertex_manager * ctx, struct gft_char c[128], struct gft_atlas * atlas, float x, float y, float sx, float sy, const char* format, ...)
{
	ASSERT_PARAM_NOTNULL (ctx);
	ASSERT_PARAM_NOTNULL (format);
	ASSERT_NOTNULL (ctx->strbuf);
	va_list arglist;
	va_start (arglist, format);
	memset (ctx->strbuf, 0, ctx->maxchars);
	vsnprintf (ctx->strbuf, ctx->maxchars, format, arglist);
	//printf ("%s", buf);
	glx_vertex_manager_drawtext (ctx, c, atlas, x, y, sx, sy, ctx->strbuf);
	va_end (arglist);
}





static void glx_vertex_manager_drawrect (struct glx_vertex_manager * ctx, float x, float y, float w, float h, float l)
{
	uint32_t const stride = 5;
	ASSERT_PARAM_NOTNULL (ctx);
	ASSERT (glIsProgram(ctx->program));
	ASSERT (glIsVertexArray(ctx->vao));
	ASSERT (glIsBuffer(ctx->vbo));

	float * v = ctx->v;
	v += (ctx->char_last * stride * 6);
	gft_trianglemesh2 (v+0, stride, x, y, w, h);
	gft_trianglemesh2 (v+2, stride, 0, 0, 1, 1);
	vf32_set1_strided (v+4, l, 6, stride);
	ctx->char_last += 1;
}


static void glx_vertex_manager_flush (struct glx_vertex_manager * ctx)
{
	uint32_t const stride = 5;
	ASSERT_PARAM_NOTNULL (ctx);
	glBindVertexArray (ctx->vao);
	//glUseProgram (ctx->program);
	//glBindTexture (GL_TEXTURE_2D, ctx->tex);
	glUniform1i (ctx->uniform_tex, 0);
	glBindBuffer(GL_ARRAY_BUFFER, ctx->vbo);
	GLsizeiptr size = sizeof(float) * stride * 6 * ctx->char_last; // Triangles vertices size in bytes
	GLsizei count = ctx->char_last * 6; //Number of triangles
	glBufferData (GL_ARRAY_BUFFER, size, ctx->v, GL_DYNAMIC_DRAW);
	glDrawArrays (GL_TRIANGLES, 0, count);
	ctx->char_last = 0;
	memset (ctx->v, 0, size);//TODO: Why do we need this?
}





struct gtext1_context
{
	FT_Library ft;
	FT_Face face;
	char const * filename;
	struct gft_char c[128];
	struct gft_atlas atlas;
};





static void gtext1_setup (struct gtext1_context * ctx)
{
	ASSERT_PARAM_NOTNULL (ctx);

	if (FT_New_Face(ctx->ft, ctx->filename, 0, &ctx->face))
	{
		XLOG (XLOG_ERR, XLOG_FT, "Failed to load font %s", ctx->filename);
		exit (1);
	}


	/*
	unsigned unit = 0;
	glActiveTexture (GL_TEXTURE0 + unit);
	glGenTextures (1, &ctx->tex);
	glBindTexture (GL_TEXTURE_2D, ctx->tex);
	glUniform1i (ctx->uniform_tex, unit);
	*/


	FT_Set_Pixel_Sizes (ctx->face, 0, 48);
	gft_init (ctx->face, ctx->c, &ctx->atlas, 512);

}




























