#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../csc_gft.h"

struct gtext2_context
{
	GLuint tex;
	GLuint vao;
	GLuint vbo_pos;
	GLuint vbo_uv;
	GLint uniform_tex;
	GLint uniform_mvp;
	GLuint program;
	struct gchar c[128];
	struct gatlas atlas;
	FT_Face face;
};


static void gtext2_context_init(struct gtext2_context * ctx, GLuint program)
{
	glUseProgram (program);
	ctx->uniform_tex = glGetUniformLocation (program, "tex0");
	ctx->uniform_mvp = glGetUniformLocation (program, "mvp");
	ctx->program = program;
	// Create a texture that will be used to hold all ASCII glyphs
	unsigned unit = 0;
	glActiveTexture (GL_TEXTURE0 + unit);
	glGenTextures (1, &ctx->tex);
	glBindTexture (GL_TEXTURE_2D, ctx->tex);
	glUniform1i (ctx->uniform_tex, unit);
	gtext_init (ctx->face, ctx->c, &ctx->atlas);

	// Set up the VBO for our vertex data
	enum attr
	{
		ATTRIBUTE_POS = 0, // vertex shader: "layout (location = 0)"
		ATTRIBUTE_UV = 1,  // vertex shader: "layout (location = 1)"
	};
	glGenBuffers (1, &ctx->vbo_pos);
	glGenBuffers (1, &ctx->vbo_uv);
	glGenVertexArrays (1, &ctx->vao);
	glBindVertexArray (ctx->vao);
	glEnableVertexAttribArray (ATTRIBUTE_POS);
	glEnableVertexAttribArray (ATTRIBUTE_UV);
	glBindBuffer (GL_ARRAY_BUFFER, ctx->vbo_pos);
	glVertexAttribPointer (ATTRIBUTE_POS, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer (GL_ARRAY_BUFFER, ctx->vbo_uv);
	glVertexAttribPointer (ATTRIBUTE_UV, 2, GL_FLOAT, GL_FALSE, 0, 0);
}


static void gtext2_context_draw(struct gtext2_context * ctx, char const * text, float x, float y, float z, float sx, float sy, float mvp[4*4])
{
	glBindVertexArray (ctx->vao);
	glUseProgram (ctx->program);
	/* Use the texture containing the atlas */
	glBindTexture (GL_TEXTURE_2D, ctx->tex);
	glUniform1i (ctx->uniform_tex, 0);
	glUniformMatrix4fv (ctx->uniform_mvp, 1, GL_FALSE, (const GLfloat *) mvp);
	render_text (text, ctx->c, ctx->atlas.w, ctx->atlas.h, ctx->vbo_pos, ctx->vbo_uv, x, y, z, sx, sy);
}
