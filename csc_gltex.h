/* SPDX-License-Identifier: GPL-2.0 */
#pragma once
#include <stddef.h> //offsetof
#include <stdint.h> //offsetof
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "csc_basic.h"
#include "csc_debug.h"

void fill_texture (uint8_t * data, int w, int h, uint32_t c, uint32_t n)
{
	memset (data, 0, w * h * c);
	/*
	for (int x = 0; x < width; ++x)
	for (int y = 0; y < height; ++y)
	{
		uint8_t * p = data + (x*4) + (y*width*4);
		p[0] = 0;
		p[1] = 0;
		p[2] = 0;
		p[3] = 0;
		p[index] = 255;
	}
	*/
	//ASSERT (index < channels);
	int x = w/2;
	int y = h/2;
	int dx = 0;
	int dy = 0;
	for (uint32_t i = 0; i < n; ++i)
	{
		x += dx;
		y += dy;
		dx += (rand() % 3) - 1;
		dy += (rand() % 3) - 1;
		dx = CLAMP (dx, -4, 4);
		dy = CLAMP (dy, -4, 4);
		if (x < 0 || x >= w){dx = -dx/2;}
		if (y < 0 || y >= h){dy = -dy/2;}
		x = CLAMP (x, 0, w-1);
		y = CLAMP (y, 0, h-1);
		int i = (x*c) + (y*w*c);
		ASSERT (i >= 0);
		data[i + 0] = 255;
	}
}


void fill_texture2 (uint8_t * data, int w, int h, int c, uint8_t value)
{
	for (int i = 0; i < w * h * c; ++i)
	{
		data[i] = value;
	}
}



struct csc_gltex
{
	uint32_t width;
	uint32_t height;
	uint32_t layers;
	uint32_t unit;
};

struct csc_gltexcontext
{
	uint32_t cap;
	GLuint tbo[100];
	struct csc_gltex tex[100];
};


void csc_gltexcontext_init (struct csc_gltexcontext * ctx)
{
	ASSERT (ctx->cap < 100);
	glGenTextures (ctx->cap, ctx->tbo);
	for (uint32_t i = 0; i < ctx->cap; ++i)
	{
		srand (i);
		uint32_t width = ctx->tex[i].width;
		uint32_t height = ctx->tex[i].height;
		uint32_t layers = ctx->tex[i].layers;
		uint32_t channels = 4;
		unsigned size = width * height * channels * sizeof(uint8_t);
		uint8_t * data = calloc (size, 1);
		glActiveTexture (GL_TEXTURE0 + ctx->tex[i].unit);
		glBindTexture (GL_TEXTURE_2D_ARRAY, ctx->tbo[i]);//Depends on glActiveTexture()
		glTexParameteri (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//Depends on glBindTexture()
		glTexParameteri (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//Depends on glBindTexture()
		glTexParameteri (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//Depends on glBindTexture()
		glTexParameteri (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//Depends on glBindTexture()
		glTexStorage3D (GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width, height, layers);//Depends on glBindTexture()
		//fill_texture (data, width, height, channels, 0);
		//fill_texture2 (data, width, height, channels, 255);

		for (uint32_t j = 0; j < layers; ++j)
		{
			fill_texture (data + (j%3), width, height, channels, 200);
			glTexSubImage3D (GL_TEXTURE_2D_ARRAY, 0, 0, 0, j, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);//Depends on glBindTexture()
		}

	}
}


void csc_gltexcontext_setparams (struct csc_gltexcontext * ctx, uint32_t tbo, uint32_t width)
{

}

