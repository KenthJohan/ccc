#pragma once

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "csc_debug.h"
#include "csc_basic.h"


#define V4F32_FORMAT "(%f %f %f %f)\n"
#define V4F32_ARGS(x) (x)[0], (x)[1], (x)[2], (x)[3]
//Matrix can be stored in column major or row major.
//Column major means that a continuous array is a column vector.
//Row major means that a continuous array is a row vector.
#define MAJR(w, r, c) (((w)*(r)) + (c))
#define MAJC(w, r, c) (((w)*(c)) + (r))
#define MAJX MAJC





#define V4_SUM(v, s)\
{\
	(s) = 0;\
	for (size_t _i = 0; _i < 4; _i++)\
	(s) += (v) [_i]\
}


#define V4_DOT(s, u, v)\
{\
	(s) = 0;\
	for (size_t _i = 0; _i < 4; _i++)\
	(s) += (u)[_i] * (v)[_i];\
}


#define V4_MUL_SCALAR(r, v, s)\
{\
	for (size_t _i = 0; _i < 4; _i++)\
	(r)[_i] = (v)[_i] * (s);\
}


// Clear vector
// (v := {x | x = 0})
#define V4_CLR(v) \
{\
	for (size_t _i = 0; _i < 4; _i++)\
	(v)[_i] = 0;\
}


//Translation vector
#define M4_TX 12
#define M4_T0 12
#define M4_TY 13
#define M4_T1 13
#define M4_TZ 14
#define M4_T2 14
#define M4_TW 15
#define M4_T3 15

//Column vectors
#define M4_V0 0
#define M4_VX 0
#define M4_V1 4
#define M4_VY 4
#define M4_V2 8
#define M4_VZ 8
#define M4_V3 12
#define M4_VT 12


#define M4_IDENTITY(m)\
{\
	for (size_t _i = 0; _i < 4; _i++)\
	for (size_t _j = 0; _j < 4; _j++)\
	(m)[_i*4 + _j] = (_i == _j ? 1.0 : 0.0);\
}


#define M4_PRINT(m, fmt)\
{\
	for (size_t _r = 0; _r < 4; _r++)\
{\
	for (size_t _c = 0; _c < 4; _c++)\
	printf (fmt, (m) [MAJX(4, _r, _c)]);\
	printf ("\n");\
	}\
}


//M = A*B
#define M4_MUL_CPY(m, a, b)\
{\
	for (size_t _c = 0; _c < 4; _c = _c + 1)\
	for (size_t _r = 0; _r < 4; _r = _r + 1)\
{\
	(m) [MAJX(4, _r, _c)] = 0.0f;\
	for(size_t _k = 0; _k < 4; _k = _k + 1)\
	(m) [MAJX(4, _r, _c)] += (a) [MAJX(4, _r, _k)] * (b) [MAJX(4, _k, _c)];\
	}\
}


#define M4_MAC(m, a, b)\
{\
	for (size_t _c = 0; _c < 4; _c = _c + 1)\
	for (size_t _r = 0; _r < 4; _r = _r + 1)\
	for (size_t _k = 0; _k < 4; _k = _k + 1)\
	(m) [MAJX(4, _r, _c)] += (a) [MAJX(4, _r, _k)] * (b) [MAJX(4, _k, _c)];\
}


//y := y + M^T v
#define M4_MAC_TRANSPOSE(y, mt, v)\
{\
	for (size_t _r = 0; _r < 4; _r = _r + 1)\
	for (size_t _c = 0; _c < 4; _c = _c + 1)\
{\
	(y) [_r] += (mt) [MAJX (4, _c, _r)] * (v) [_c];\
	}\
}


#define M4_ROTX(m, a)\
(m)[5] = cos (a);\
(m)[6] = sin (a);\
(m)[9] = -sin (a);\
(m)[10] = cos (a);\

#define M4_ROTY(m, a)\
(m)[0] = cos (a);\
(m)[2] = -sin (a);\
(m)[8] = sin (a);\
(m)[10] = cos (a);\

#define M4_ROTZ(m, a)\
(m)[0] = cos (a);\
(m)[1] = sin (a);\
(m)[4] = -sin (a);\
(m)[5] = cos (a);\


#define V4_SET(v,x,y,z,w)\
{\
(v)[0] = x;\
(v)[1] = y;\
(v)[2] = z;\
(v)[3] = w;\
}\


#define VN_HADAMARD_PRODUCT(n, r, a, b)\
{\
	for (size_t _i = 0; _i < (n); _i++)\
	(r)[_i] = (a)[_i] * (b)[_i];\
}


//y := y + M v
#define MN_MAC(w, h, y, m, v)\
{\
	for (size_t _r = 0; _r < h; _r = _r + 1)\
	for (size_t _c = 0; _c < w; _c = _c + 1)\
{\
	(y) [_r] += (m) [MAJX (w, _r, _c)] * (v) [_c];\
	}\
}


//y := y + M^T v
#define MN_MAC_TRANSPOSE(w, h, y, mt, v)\
{\
	for (size_t _r = 0; _r < h; _r = _r + 1)\
	for (size_t _c = 0; _c < w; _c = _c + 1)\
{\
	(y) [_r] += (mt) [MAJX (w, _c, _r)] * (v) [_c];\
	}\
}


#define MM_MUL(w, m, a, b)\
{\
	for (size_t _c = 0; _c < w; _c = _c + 1)\
	for (size_t _r = 0; _r < w; _r = _r + 1)\
{\
	(m) [MAJX (w, _r, _c)] = 0.0f;\
	for(size_t _k = 0; _k < w; _k = _k + 1)\
	(m) [MAJX (w, _r, _c)] += (a) [MAJX (w, _r, _k)] * (b) [MAJX (w, _k, _c)];\
	}\
}


// Clear vector
// (v := {x | x = 0})
#define VN_CLR(n, v) \
{\
	for (size_t _i = 0; _i < (n); _i++)\
	(v)[_i] = 0;\
}


//Translation vector
//Independent from row/col major
#define M4_TX 12
#define M4_T0 12
#define M4_TY 13
#define M4_T1 13
#define M4_TZ 14
#define M4_T2 14
#define M4_TW 15
#define M4_T3 15


//Column vectors
//Independent from row/col major
#define M4_V0 0
#define M4_VX 0
#define M4_V1 4
#define M4_VY 4
#define M4_V2 8
#define M4_VZ 8
#define M4_V3 12
#define M4_VT 12


//Scale scalars
//Independent from row/col major
#define M4_S0 0
#define M4_S1 5
#define M4_S2 10
#define M4_S3 15



void mf32_print (float m[], unsigned rn, unsigned cn, FILE * f)
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
static void mf32_set (float * M, float c, unsigned n)
{
	for (unsigned i = 0; i < n; ++ i)
	{
		M [i] = c;
	}
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


static void vsf32_macc (float vy[], float const vx[], float sb, unsigned n)
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
		vsf32_macc (vy, ma, vx [i], rn);
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
