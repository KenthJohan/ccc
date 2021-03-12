/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "csc_debug.h"
#include "csc_basic.h"


//Column 0
#define M3_00 0
#define M3_10 1
#define M3_20 2
//Column 1
#define M3_01 3
#define M3_11 4
#define M3_21 5
//Column 2
#define M3_02 6
#define M3_12 7
#define M3_22 8

typedef float m4f32[16];
typedef float m3f32[9];
typedef float v2f32[2];
typedef float v3f32[3];
typedef float v4f32[4];
typedef float qf32[4];


struct qf32_xyzw
{
	float x;
	float y;
	float z;
	float w;
};

struct v4f32_xyzw
{
	float x;
	float y;
	float z;
	float w;
};

struct v3f32_xyz
{
	float x;
	float y;
	float z;
};

struct v2f32_xy
{
	float x;
	float y;
	float z;
};

struct v2f32_wh
{
	float w;
	float h;
};


struct v4f32_line
{
	float a[4];
	float b[4];
};

struct u32_line
{
	uint32_t a;
	uint32_t b;
};


static void mf32_print (float m[], unsigned rn, unsigned cn, FILE * f)
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

static void vf32_set1_strided (float v[], float x, unsigned n, unsigned inc)
{
	for (unsigned i = 0; i < n; ++i)
	{
		v[0] = x;
		v += inc;
	}
}


static void vf32_set2 (float v[], float v0, float v1)
{
	v[0] = v0;
	v[1] = v1;
}


static void vf32_set3 (float v[], float v0, float v1, float v2)
{
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;
}


static void vf32_set4 (float v[], float v0, float v1, float v2, float v3)
{
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;
	v[3] = v3;
}



// r := a . b
static float vvf32_dot (unsigned n, float const a [], float const b [])
{
	float r = 0.0f;
	while (n--)
	{
		r += a [n] * b [n];
	}
	return r;
}


static void vsf32_macc (unsigned n, float vy[], float const vx[], float sb)
{
	for (unsigned i = 0; i < n; ++i)
	{
		vy [i] += vx [i] * sb;
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
		vy[i] += vvf32_dot (rn, ma, vx);
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


static void vf32_cpy (unsigned n, float des [], float const src [])
{
	while (n--)
	{
		des [n] = src [n];
	}
}


// r := a + b
static void vvf32_add (unsigned n, float r [], float const a [], float const b [])
{
	while (n--)
	{
		r [n] = a [n] + b [n];
	}
}


// r := a - b
static void vvf32_sub (unsigned n, float r [], float const a [], float const b [])
{
	while (n--)
	{
		r [n] = a [n] - b [n];
	}
}

// r := r + a
static void vf32_acc (unsigned n, float r [], float const a [])
{
	vvf32_add (n, r, r, a);
}


// r := r - a
static void vf32_decc (unsigned n, float r [], float const a [])
{
	vvf32_sub (n, r, r, a);
}


// r := a + b
static void vsf32_add (unsigned n, float r [], float const a [], float const b)
{
	while (n--)
	{
		r [n] = a [n] + b;
	}
}


// r := a + b
static void vsf32_add_max (unsigned n, float r [], float const a [], float const b, float max)
{
	while (n--)
	{
		if (r [n] < max)
		{
			r [n] = a [n] + b;
		}
	}
}


// r := a - b
static void vsf32_sub (unsigned n, float r [], float const a [], float b)
{
	while (n--)
	{
		r [n] = a [n] - b;
	}
}


// r := a < b
static void vvf32_lt (unsigned n, float r [], float const a [], float const b [])
{
	while (n--)
	{
		r [n] = a [n] < b [n];
	}
}


// r := a > b
static void vvf32_gt (unsigned n, float r [], float const a [], float const b [])
{
	while (n--)
	{
		r [n] = a [n] > b [n];
	}
}


// ret a > b
static int vf32_gt (unsigned n, float const a [], float const b [])
{
	while (n--)
	{
		if (a [n] <= b [n])
		{
			return 0;
		}
	}
	return 1;
}


// ret a < b
static int vf32_lt (unsigned n, float const a [], float const b [])
{
	while (n--)
	{
		if (a [n] <= b [n]) {return 0;}
	}
	return 1;
}


// Set all element (x) of r to b
// r := {x | x = ?}
static void vf32_random (unsigned n, float r [])
{
	while (n--)
	{
		r [n] = (float)rand () / (float)RAND_MAX;
		//r [i] = 1.0f;
	}
}


// Set all element (x) of r to b
// r := {x | x = b}
static void vf32_set1 (unsigned n, float r [], float const b)
{
	while (n--)
	{
		r [n] = b;
	}
}


// r := a * b
static void vvf32_hadamard (unsigned n, float r [], float const a [], float const b [])
{
	while (n--)
	{
		r [n] = a [n] * b [n];
	}
}


// r := r + a * b
void vvf32_macc (uint32_t n, float r [], float const a [], float const b [])
{
	while (n--)
	{
		r [n] += a [n] * b [n];
	}
}


// r := a * b
void vsf32_mul (uint32_t n, float r [], float const a [], float const b)
{
	while (n--)
	{
		r [n] = a [n] * b;
	}
}



float vf32_sum (uint32_t n, float const v [])
{
	float sum = 0;
	while (n--)
	{
		sum += v [n];
	}
	return sum;
}


// ret |a|^2
float vf32_norm2 (uint32_t n, float const a [])
{
	return vvf32_dot (n, a, a);
}


// ret |a|
float vf32_norm (uint32_t n, float const a [])
{
	return sqrtf (vf32_norm2 (n, a));
}


// r := a / |a|
void vf32_normalize (uint32_t n, float r [], float const a [])
{
	float const l = vf32_norm (n, a);
	float const s = l > 0.0f ? 1.0f / l : 0.0f;
	vsf32_mul (n, r, a, s);
}


void vf32_linespace (uint32_t n, float x [], float x1, float x2)
{
	float const d = (x2 - x1) / n;
	float a = x1;
	while (n--)
	{
		x [n] = a;
		a += d;
	}
}


void vf32_print (FILE * f, float const x [], size_t n, char const * format)
{
	fprintf (f, "(");
	for (size_t i = 0; i < n; ++ i)
	{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
		fprintf (f, format, (double) x [i]);
#pragma GCC diagnostic pop
	}
	fprintf (f, "\b)\n");
	fflush (f);
}


void vf32_print2 (FILE * f, float const x [], size_t n1, size_t n2, char const * format)
{
	for (size_t i = 0; i < n1; ++ i)
	{
		vf32_print (f, x + (i*n2), n2, format);
	}
}


void vf32_setl (float r [], uint32_t n, ...)
{
	va_list ap;
	va_start (ap, n);
	for (uint32_t i = 0; i < n; ++i)
	{
		r [i] = va_arg (ap, double);
	}
	va_end (ap);
}


void vu32_setl (uint32_t r [], uint32_t n, ...)
{
	va_list ap;
	va_start (ap, n);
	for (uint32_t i = 0; i < n; ++i)
	{
		r [i] = va_arg (ap, uint32_t);
	}
	va_end (ap);
}


void vu32_set1 (uint32_t n, uint32_t r [], uint32_t v)
{
	while (n--)
	{
		r [n] = v;
	}
}


void vu32_and1 (uint32_t n, uint32_t r [], uint32_t a [], uint32_t b)
{
	while (n--)
	{
		r [n] = a [n] & b;
	}
}


// r := a + b
void vu32_add1max (uint32_t n, uint32_t r [], uint32_t const a [], uint32_t b, uint32_t max)
{
	while (n--)
	{
		if (r [n] < max)
		{
			r [n] = a [n] + b;
		}
	}
}


// r := a + b
void vu32_add1min (uint32_t n, uint32_t r [], uint32_t const a [], uint32_t b, uint32_t min)
{
	while (n--)
	{
		if (r [n] > min)
		{
			r [n] = a [n] + b;
		}
	}
}



void vu32_ladder (uint32_t n, uint32_t x [], uint32_t const d [])
{
	uint32_t h = 0;
	//Must be forward iteration
	for (uint32_t i = 0; i < n; ++ i)
	{
		x [i] = h;
		h += d [i];
	}
}


void vu32_linespace (uint32_t n, uint32_t x [], uint32_t x1, uint32_t x2)
{
	float d = ((float)x2 - (float)x1) / n;
	float a = (float)x1;
	while (n--)
	{
		x [n] = (uint32_t)a;
		a += d;
	}
}


void vf32_repeat (uint32_t n, float v [], float value, uint32_t offset, uint32_t stride)
{
	v += offset;
	while (n--)
	{
		v [0] = value;
		v += stride;
	}
}


void vf32_weight_ab (uint32_t n, float y [], float a [], float b [], float k)
{
	float A = k;
	float B = 1.0f - k;
	while (n--)
	{
		y [n] = A * a [n] + B * b [n];
	}
}





//Linearly map (x) value from (A0 .. A1) to (B0 .. B1)
float sf32_linmap (float X, float A0, float A1, float B0, float B1)
{
	//cropping
	if (X < A0) {return B0;}
	if (X > A1) {return B1;}
	//calculate delta
	float DA;
	float DB;
	DA = A1 - A0;
	DB = B1 - B0;
	//move to zero
	X = X - A0;
	//zero division protection
	if (DA == 0) {return B1;};
	X = X / DA;
	//new scale
	X = X * DB;
	//new offset
	X = X + B0;
	return X;
}



static void vf32_addv (uint32_t dim, float y[], uint32_t y_stride, float const a[], uint32_t a_stride, float const b[], uint32_t b_stride, uint32_t n)
{
	for (uint32_t i = 0; i < n; ++i)
	{
		vvf32_add (dim, y, a, b);
		y += y_stride;
		a += a_stride;
		b += b_stride;
	}
}


static void vf32_subv (uint32_t dim, float y[], uint32_t y_stride, float const a[], uint32_t a_stride, float const b[], uint32_t b_stride, uint32_t n)
{
	for (uint32_t i = 0; i < n; ++i)
	{
		vvf32_sub (dim, y, a, b);
		y += y_stride;
		a += a_stride;
		b += b_stride;
	}
}



void vf32_convolution1d (float const q[], uint32_t n, float u[], float k[], uint32_t kn)
{
	uint32_t kn0 = kn / 2;
	vf32_normalize (kn, k, k);
	for (uint32_t i = kn0; i < n-kn0; ++i)
	{
		float sum = 0.0f;
		for (uint32_t j = 0; j < kn; ++j)
		{
			sum += q[i - kn0 + j] * k[j];
		}
		u[i] = sum;
	}
}


static void vf32_convolution2d (float pix2[], float const pix[], int32_t xn, int32_t yn, float k[], int32_t kxn, int32_t kyn)
{
	int32_t kxn0 = kxn / 2;
	int32_t kyn0 = kyn / 2;
	//printf ("kxn0 %i\n", kxn0);
	//printf ("kyn0 %i\n", kyn0);
	for (int32_t y = kyn0; y < (yn-kyn0); ++y)
	{
		for (int32_t x = kxn0; x < (xn-kxn0); ++x)
		{
			float sum = 0.0f;
			for (int32_t ky = 0; ky < kyn; ++ky)
			{
				for (int32_t kx = 0; kx < kxn; ++kx)
				{
					int32_t xx = x + kx - kxn0;
					int32_t yy = y + ky - kyn0;
					sum += pix[yy * xn + xx] * k[ky * kxn + kx];
				}
			}
			pix2[y * xn + x] = sum;
			//pix2[y * xn + x] = pix[y * xn + x];
		}
	}
}


static void vf32_convolution2d_masked (float pix2[], float const pix[], float const mask[], int32_t xn, int32_t yn, float k[], int32_t kxn, int32_t kyn)
{
	int32_t kxn0 = kxn / 2;
	int32_t kyn0 = kyn / 2;
	//printf ("kxn0 %i\n", kxn0);
	//printf ("kyn0 %i\n", kyn0);
	for (int32_t y = kyn0; y < (yn-kyn0); ++y)
	{
		for (int32_t x = kxn0; x < (xn-kxn0); ++x)
		{
			float sum = 0.0f;
			for (int32_t ky = 0; ky < kyn; ++ky)
			{
				for (int32_t kx = 0; kx < kxn; ++kx)
				{
					int32_t xx = x + kx - kxn0;
					int32_t yy = y + ky - kyn0;
					int32_t index = yy * xn + xx;
					sum += mask[index] * pix[index] * k[ky * kxn + kx];
				}
			}
			pix2[y * xn + x] = sum;
			//pix2[y * xn + x] = pix[y * xn + x];
		}
	}
}


static void vf32_convolution2d_clean (float pix2[], float const pix[], int32_t xn, int32_t yn, int32_t kxn, int32_t kyn)
{
	int32_t kxn0 = kxn / 2;
	int32_t kyn0 = kyn / 2;
	//printf ("kxn0 %i\n", kxn0);
	//printf ("kyn0 %i\n", kyn0);
	for (int32_t y = kyn0; y < (yn-kyn0); ++y)
	{
		for (int32_t x = kxn0; x < (xn-kxn0); ++x)
		{
			float sum = 1.0f;
			for (int32_t ky = 0; ky < kyn; ++ky)
			{
				for (int32_t kx = 0; kx < kxn; ++kx)
				{
					int32_t xx = x + kx - kxn0;
					int32_t yy = y + ky - kyn0;
					int32_t index = yy * xn + xx;
					sum *= pix[index];
				}
			}
			pix2[y * xn + x] = sum;
			//pix2[y * xn + x] = pix[y * xn + x];
		}
	}
}



uint32_t vf32_amount_positive (float q[], uint32_t qn, uint32_t * qi, uint32_t count)
{
	uint32_t n = 0;
	while (count--)
	{
		if ((*qi) >= qn){break;}
		if (q[(*qi)] < 0.0f){break;}
		(*qi)++;
		n++;
	}
	return n;
}


uint32_t vf32_amount_negative (float q[], uint32_t qn, uint32_t * qi, uint32_t count)
{
	uint32_t n = 0;
	while (count--)
	{
		if ((*qi) >= qn){break;}
		if (q[(*qi)] > 0.0f){break;}
		(*qi)++;
		n++;
	}
	return n;
}



void vf32_remove_low_values (float q[], uint32_t qn)
{
	float pos = 0.0f;
	float neg = 0.0f;
	float pos_n = 0;
	float neg_n = 0;
	for (uint32_t i = 0; i < qn; ++i)
	{
		if (q[i] > 0.0f)
		{
			pos += q[i];
			pos_n += 1.0f;
		}
		else if (q[i] < 0.0f)
		{
			neg += q[i];
			neg_n += 1.0f;
		}
	}
	pos /= pos_n;
	neg /= neg_n;
	for (uint32_t i = 0; i < qn; ++i)
	{
		if ((q[i] > 0.0f) && (q[i] < pos))
		{
			q[i] = 0.0f;
		}
		if ((q[i] < 0.0f) && (q[i] > neg))
		{
			q[i] = 0.0f;
		}
	}
}


uint32_t vf32_skip_zero (float q[], uint32_t qn, uint32_t * qi)
{
	uint32_t n = 0;
	while (1)
	{
		if ((*qi) >= qn){break;}
		if (q[(*qi)] != 0.0f){break;}
		(*qi)++;
		n++;
	}
	return n;
}


/**
 * @brief Find peaks.
 * @param q[in,out] The array
 * @param qn[in]    Number of elements in \p q
 * @param g[out]    The index where the peak is located
 * @param gn[in]    Number of elements in \p g
 */
void vf32_find_peaks (float q[], uint32_t qn, uint32_t g[], uint32_t gn, uint32_t r, uint32_t margin)
{
	for (uint32_t gi = 0; gi < gn; ++gi)
	{
		float qmax = 0.0f;
		uint32_t qimax = 0;
		for (uint32_t qi = margin; qi < qn-margin; ++qi)
		{
			if (qmax < q[qi])
			{
				qmax = q[qi];
				qimax = qi;
			}
		}
		g[gi] = qimax;
		uint32_t qi0 = r <= qimax ? (qimax-r) : 0;
		uint32_t qi1 = MIN (qimax+r, qn);
		for (uint32_t qi = qi0; qi < qi1; ++qi)
		{
			q[qi] = 0.0f;
		}
	}
}











/**
 * @brief Perfoms (xx^t) which outputs a symmetric matrix
 * @param m
 * @param x
 * @param x_stride
 * @param x_count
 */
void mf32_symmetric_xxt (uint32_t dim, float m[], uint32_t m_stride, float x[], uint32_t x_stride, uint32_t x_count)
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


void vf32_move_center_to_zero (uint32_t dim, float const x[], uint32_t ldx, float y[], uint32_t ldy, uint32_t n, float mean[3])
{
	memset (mean, 0, sizeof (float)*dim);
	//Calculate the (mean) coordinate from (v):
	vf32_addv (dim, mean, 0, mean, 0, x, ldx, n);
	vsf32_mul (dim, mean, mean, 1.0f / (float)n);
	//Move all (v) points to origin using coordinate (mean):
	vf32_subv (dim, y, ldy, x, ldx, mean, 0, n);
}


void mf32_get_covariance (uint32_t dim, float v[], uint32_t v_stride, uint32_t n, float c[3*3])
{
	//Calculate the covariance matrix (c) from (v):
	memset (c, 0, sizeof (float)*dim*dim);
	mf32_symmetric_xxt (3, c, 3, v, v_stride, n);
	vsf32_mul (dim*dim, c, c, 1.0f / ((float)n - 1.0f));
}



static float f32_lerp1 (float a, float b, float t)
{
	return a + t * (b - a);
}


static float f32_lerp2 (float a, float b, float t)
{
	return (1 - t) * a + t * b;
}



static float vf32_avg (uint32_t n, float v[])
{
	ASSERT (n > 0);
	float sum = 0.0f;
	for (uint32_t i = 0; i < n; ++i)
	{
		sum += v[i];
	}
	return sum / n;
}





