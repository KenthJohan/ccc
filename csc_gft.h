#pragma once
#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "csc_xlog.h"
#include "csc_math.h"
#include "csc_primf32.h"


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
	*width = w;
	*height = h;
}


static int gft_init
(FT_Face face, struct gft_char c[], struct gft_atlas * atlas, unsigned int maxwidth, FT_UInt pixel_width, FT_UInt pixel_height)
{
	ASSERT (pixel_width <= 128);
	ASSERT (pixel_height <= 128);
	FT_Set_Pixel_Sizes (face, pixel_width, pixel_height);
	XLOG (XLOG_INF, XLOG_GENERAL, "FT_HAS_COLOR: %d", FT_HAS_COLOR(face));
	XLOG (XLOG_INF, XLOG_GENERAL, "FT_Set_Pixel_Sizes: %d %d", pixel_width, pixel_height);

	FT_GlyphSlot g = face->glyph;
	unsigned int w = 0;
	unsigned int h = 0;
	gft_size (face, maxwidth, &w, &h);
	XLOG (XLOG_INF, XLOG_GENERAL, "atlas requirement: %i, %i\n", w, h);
	w = 512;
	h = 512;



	// Paste all glyph bitmaps into the texture, remembering the offset
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

		uint32_t rgba[128*128];
		for (uint32_t j = 0; j < g->bitmap.width * g->bitmap.rows; j++)
		{
			rgba[j] = (g->bitmap.buffer[j] << 0) | (g->bitmap.buffer[j] << 8) | (g->bitmap.buffer[j] << 16) | (g->bitmap.buffer[j] << 24);
		}

		GLint level = 0;
		GLint zoffset = 0;
		GLsizei depth = 1;
		glTexSubImage3D (GL_TEXTURE_2D_ARRAY, level, ox, oy, zoffset, g->bitmap.width, g->bitmap.rows, depth, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
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













static uint32_t gft_gen_pos
(float pos[], uint32_t n, uint32_t stride, const char *text, struct gft_char c[], float x, float y, float sx, float sy)
{
	uint32_t i = 0;
	uint8_t const *p;
	for (p = (const uint8_t *)text; *p && (n > 0); p++, --n)
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
		primf32_make_rectangle (pos, stride, x2, y2, w, h);
		pos += stride * 6; //gft_trianglemesh2 writes 6 vertices where each vertex have (stride)
		i += 6;
	}
	return i;
}


static uint32_t gft_gen_uv
(float uv[], uint32_t n, uint32_t stride, const char *text, struct gft_char c[], float aw, float ah)
{
	uint32_t i = 0;
	uint8_t const *p;
	for (p = (const uint8_t *)text; *p && (n > 0); p++, --n)
	{
		// Calculate texture coordinates
		float tx = c[*p].tx;
		float ty = c[*p].ty;
		float tw = c[*p].bw / aw;
		float th = c[*p].bh / ah;
		// Skip glyphs that have no pixels
		if (!c[*p].bw || !c[*p].bh) {continue;}
		primf32_make_rectangle (uv, stride, tx, ty, tw, th);
		uv += stride * 6;
		i += 6;
	}
	return i;
}



















