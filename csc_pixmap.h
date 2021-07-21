/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once
#include <stddef.h> //offsetof
#include <stdint.h> //offsetof

#include "csc/csc_xlog.h"
#include "csc/csc_math.h"


static void csc_pixmap_drawrect (uint32_t * image, int32_t iw, int32_t ih, int32_t rx, int32_t ry, int32_t rw, int32_t rh, uint32_t value)
{
	ASSERT_NOTNULL (image);
	rx += MIN (0, rw); //Handle negative width
	ry += MIN (0, rh); //Handle negative height
	rw = abs (rw); //Handle negative width
	rh = abs (rh); //Handle negative height
	rw = MIN (rw + rx, iw);
	rh = MIN (rh + ry, ih);
	for (int32_t y = ry; y < rh; ++y)
	{
		for (int32_t x = rx; x < rw; ++x)
		{
			ASSERT (x >= 0);
			ASSERT (y >= 0);
			ASSERT_LT (x, iw);
			ASSERT_LT (y, ih);
			int32_t i = x + y*ih;
			ASSERT (i >= 0);
			ASSERT (i < (iw*ih));
			image[i] = value;
		}

	}
}


static void csc_pixmap_plot_i32 (uint32_t * image, uint32_t w, uint32_t h, int32_t data[], uint32_t n, uint32_t value)
{
	ASSERT_NOTNULL (image);
	ASSERT_NOTNULL (data);
	for (uint32_t i = 0; i < n; ++i)
	{
		csc_pixmap_drawrect (image, w, h, i, h/2, 1, data[i], value);
	}
}

static void csc_pixmap_plot_u8 (uint32_t * image, uint32_t w, uint32_t h, uint8_t data[], uint32_t n, uint32_t value)
{
	ASSERT_NOTNULL (image);
	ASSERT_NOTNULL (data);
	for (uint32_t i = 0; i < n; ++i)
	{
		csc_pixmap_drawrect (image, w, h, i, 0, 1, data[i], value);
	}
}
