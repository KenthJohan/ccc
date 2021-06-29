#pragma once
#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "csc_xlog.h"
#include "csc_math.h"


struct gft_char
{
	float ax;	// advance.x
	float ay;	// advance.y
	float bw;	// bitmap.width;
	float bh;	// bitmap.height;
	float bl;	// bitmap_left;
	float bt;	// bitmap_top;
	float tx;	// x offset of glyph in texture coordinates
	float ty;	// y offset of glyph in texture coordinates
};


struct gft_atlas
{
	unsigned int w;
	unsigned int h;
};


static void gft_size (FT_Face face, unsigned int maxwidth, unsigned int * width, unsigned int * height)
{
	FT_GlyphSlot g = face->glyph;
	unsigned int w = 0;
	unsigned int h = 0;
	unsigned int roww = 0;
	unsigned int rowh = 0;

	/* Find minimum size for a texture holding all visible ASCII characters */
	for (int i = 32; i < 128; i++)
	{
		if (FT_Load_Char (face, i, FT_LOAD_RENDER))
		{
			XLOG (XLOG_ERR, XLOG_GENERAL, "Loading character %c failed!\n", i);
			continue;
		}

		if (roww + g->bitmap.width + 1 >= maxwidth)
		{
			w = MAX(w, roww);
			h += rowh;
			roww = 0;
			rowh = 0;
		}
		roww += g->bitmap.width + 1;
		rowh = MAX(rowh, g->bitmap.rows);
	}
	w = MAX(w, roww);
	h += rowh;
	printf ("%i, %i\n", w, h);
	*width = w;
	*height = h;
}


static int gft_init
(FT_Face face, struct gft_char c[], struct gft_atlas * atlas, unsigned int maxwidth)
{
	FT_GlyphSlot g = face->glyph;
	unsigned int w = 0;
	unsigned int h = 0;
	gft_size (face, maxwidth, &w, &h);
	w = 512;
	h = 512;

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, w, h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);
	/* We require 1 byte alignment when uploading texture data */
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	/* Clamping to edges is important to prevent artifacts when scaling */
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	/* Linear filtering usually looks best for text */
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	/* Paste all glyph bitmaps into the texture, remembering the offset */
	int ox = 0;
	int oy = 0;
	unsigned int rowh = 0;

	for (int i = 32; i < 128; i++)
	{
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
		{
			XLOG (XLOG_ERR, XLOG_GENERAL, "Loading character %c failed!", i);
			continue;
		}

		if (ox + g->bitmap.width + 1 >= maxwidth)
		{
			oy += rowh;
			rowh = 0;
			ox = 0;
		}

		//glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
		GLint level = 0;
		GLint zoffset = 0;
		GLsizei depth = 1;
		glTexSubImage3D (GL_TEXTURE_2D_ARRAY, level, ox, oy, zoffset, g->bitmap.width, g->bitmap.rows, depth,  GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
		printf ("%c: %4i %4i %4i %4i\n", i, ox, oy, g->bitmap.width, g->bitmap.rows);
		c[i].ax = g->advance.x >> 6;
		c[i].ay = g->advance.y >> 6;
		c[i].bw = g->bitmap.width;
		c[i].bh = g->bitmap.rows;
		c[i].bl = g->bitmap_left;
		c[i].bt = g->bitmap_top;
		c[i].tx = ox / (float)w;
		c[i].ty = oy / (float)h;
		rowh = MAX(rowh, g->bitmap.rows);
		ox += g->bitmap.width + 1;
	}

	atlas->w = w;
	atlas->h = h;
	return 0;
}






static void gft_trianglemesh2 (float v[], uint32_t stride, float x, float y, float w, float h)
{
	v[0] = x + 0; v[1] = y + h; v += stride;
	v[0] = x + w; v[1] = y + 0; v += stride;
	v[0] = x + 0; v[1] = y + 0; v += stride;
	v[0] = x + w; v[1] = y + 0; v += stride;
	v[0] = x + 0; v[1] = y + h; v += stride;
	v[0] = x + w; v[1] = y + h; v += stride;
}







static uint32_t gft_gen_pos
(float pos[], uint32_t n, uint32_t stride, const char *text, struct gft_char c[], float x, float y, float sx, float sy)
{
	uint32_t i = n;
	uint8_t const *p;
	for (p = (const uint8_t *)text; *p && (i > 0); p++, --i)
	{
		// Calculate position coordinates
		float x2 = x + c[*p].bl * sx;
		float y2 = y + c[*p].bt * sy;
		float w = c[*p].bw * sx;
		float h = c[*p].bh * -sy;
		// Advance the cursor to the start of the next character
		x += c[*p].ax * sx;
		y += c[*p].ay * sy;
		// Skip glyphs that have no pixels */
		if (!w || !h) {continue;}
		gft_trianglemesh2 (pos, stride, x2, y2, w, h);
		pos += stride * 6; //gft_trianglemesh2 writes 6 vertices where each vertex have (stride)
	}
	return n-i;
}


static uint32_t gft_gen_uv
(float uv[], uint32_t n, uint32_t stride, const char *text, struct gft_char c[], float aw, float ah)
{
	uint32_t i = n;
	uint8_t const *p;
	for (p = (const uint8_t *)text; *p && (i > 0); p++, --i)
	{
		// Calculate texture coordinates
		float tx = c[*p].tx;
		float ty = c[*p].ty;
		float tw = c[*p].bw / aw;
		float th = c[*p].bh / ah;
		// Skip glyphs that have no pixels */
		if (!c[*p].bw || !c[*p].bh) {continue;}
		gft_trianglemesh2 (uv, stride, tx, ty, tw, th);
		uv += stride * 6; //gft_trianglemesh2 writes 6 vertices where each vertex have (stride)
	}
	return n-i;
}



















