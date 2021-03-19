/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdint.h>
#include "csc_math.h"
#include "csc_vf32.h"



static void mf32_print (float const m[], unsigned rn, unsigned cn, FILE * f)
{
	for (unsigned r = 0; r < rn; ++ r)
	{
		for (unsigned c = 0; c < cn; ++ c)
		{
			//double v = m [c*rn+r];
			fprintf (f, "%+10.3f ", (double) m [c*rn+r]);
		}
		fprintf (f, "\n");
	}
	fprintf (f, "\n");
	fflush (f);
}



// Set all element (x) of (M) to a constant (c)
// M := {x is f32 | x = c}
static void mf32_set (float m[], float c, unsigned n)
{
	for (unsigned i = 0; i < n; ++ i)
	{
		m [i] = c;
	}
}



static void mvf32_macc (float vy[], float const ma[], float const vx[], unsigned rn, unsigned cn)
{
	for (unsigned i = 0; i < cn; ++i)
	{
		vsf32_macc (rn, vy, ma, vx [i]);
		ma += rn;
	}
}


static void mvf32_macc_transposed (float vy[], float const ma[], float const vx[], unsigned rn, unsigned cn)
{
	for (unsigned i = 0; i < cn; ++i)
	{
		vy[i] += vf32_dot (rn, ma, vx);
		ma += cn;
	}
}




static void mmf32_macc (float y[], float const a[], float const b[], unsigned rn, unsigned n, unsigned cn)
{
	ASSERT (y != a);
	ASSERT (y != b);
	for (unsigned c = 0; c < cn; ++c)
	{
		mvf32_macc (y, a, b, rn, n);
		y += rn;
		b += n;
	}
}



/**
 * @brief Perfoms (xx^t) which outputs a symmetric matrix
 * @param m
 * @param x
 * @param x_stride
 * @param x_count
 */
static void mf32_symmetric_xxt (uint32_t dim, float m[], uint32_t m_stride, float x[], uint32_t x_stride, uint32_t x_count)
{
	for (uint32_t i = 0; i < x_count; ++i)
	{
		for (uint32_t j = 0; j < dim; ++j)
		{
			vsf32_macc (dim, m + m_stride*j, x, x[j]);
		}



		/*
		m[0] += (x[0]) * (x[0]);
		m[1] += (x[0]) * (x[1]);
		m[2] += (x[0]) * (x[2]);

		m[3] += (x[1]) * (x[0]);
		m[4] += (x[1]) * (x[1]);
		m[5] += (x[1]) * (x[2]);

		m[6] += (x[2]) * (x[0]);
		m[7] += (x[2]) * (x[1]);
		m[8] += (x[2]) * (x[2]);
		*/

		x += x_stride;
	}
}


static void mf32_get_covariance (uint32_t dim, float v[], uint32_t v_stride, uint32_t n, float c[3*3])
{
	//Calculate the covariance matrix (c) from (v):
	memset (c, 0, sizeof (float)*dim*dim);
	mf32_symmetric_xxt (3, c, 3, v, v_stride, n);
	vsf32_mul (dim*dim, c, c, 1.0f / ((float)n - 1.0f));
}
