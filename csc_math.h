#pragma once

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "csc_debug.h"
#include "csc_basic.h"


void mf32_print (float * M, unsigned rn, unsigned cn, FILE * f)
{
	for (unsigned r = 0; r < rn; ++ r)
	{
		for (unsigned c = 0; c < cn; ++ c)
		{
			fprintf (f, "%f ", (double) M [c*rn+r]);
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
static void vvf32_sub (unsigned n, float r [], float const a [], float const b [])
{
	while (n--)
	{
		r [n] = a [n] - b [n];
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


int v3f32_ray_sphere_intersect
(float p [3], float d [3], float sc [3], float sr, float *t, float q [3])
{
	//Vector m = p - s.c;
	float m [3];
	vvf32_sub (3, p, sc, m);
	//float b = Dot(m, d);
	float b = vvf32_dot (3, m, d);
	//float c = Dot(m, m) - s.r * s.r;
	float c = vvf32_dot (3, m, m) - (sr * sr);

	// Exit if r’s origin outside s (c > 0) and r pointing away from s (b > 0)
	if (c > 0.0f && b > 0.0f) {return 0;}
	float discr = (b * b) - c;

	// A negative discriminant corresponds to ray missing sphere
	if (discr < 0.0f) {return 0;}

	// Ray now found to intersect sphere, compute smallest t value of intersection
	*t = -b - sqrtf (discr);

	// If t is negative, ray started inside sphere so clamp t to zero
	if (*t < 0.0f) {*t = 0.0f;}
	*t = MAX (*t, 0.0f);

	//q = p + dt;
	vsf32_mul (3, q, d, *t);
	vvf32_add (3, q, p, q);

	return 1;
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



void v3f32_cross (float r[3], float a[3], float b[3])
{
	r[0] = a[1] * b[2] - a[2] * b[1];
	r[1] = a[2] * b[0] - a[0] * b[2];
	r[2] = a[0] * b[1] - a[1] * b[0];
}


void v3f32_crossacc (float r[3], float a[3], float b[3])
{
	r[0] += a[1] * b[2] - a[2] * b[1];
	r[1] += a[2] * b[0] - a[0] * b[2];
	r[2] += a[0] * b[1] - a[1] * b[0];
}

void v3f32_crossacc_scalar (float r[], float s, float const a[], float const b[])
{
	r[0] += s * (a[1] * b[2] - a[2] * b[1]);
	r[1] += s * (a[2] * b[0] - a[0] * b[2]);
	r[2] += s * (a[0] * b[1] - a[1] * b[0]);
}


void v4f32_cpy (float r [4], float const a [4])
{
	vf32_cpy (4, r, a);
}

void v4f32_add (float r [4], float const a [4], float const b [4])
{
	vvf32_add (4, r, a, b);
}


void v4f32_sub (float r [4], float const a [4], float const b [4])
{
	vvf32_sub (4, r, a, b);
}


void v4f32_set1 (float r [4], float const b)
{
	vf32_set1 (4, r, b);
}


void v4f32_mul (float r [4], float const a [4], float const b)
{
	vsf32_mul (4, r, a, b);
}


void v4f32_normalize (float r [4], float const a [4])
{
	vf32_normalize (4, r, a);
}


float v4f32_norm2 (float const a [4])
{
	return vvf32_dot (4, a, a);
}


//xyzw xyzw xyzw ... n times
void v4f32_repeat4 (uint32_t n, float r [], float x, float y, float z, float w)
{
	while (n--)
	{
		//TRACE_F ("%i", n);
		r [0] = x;
		r [1] = y;
		r [2] = z;
		r [3] = w;
		r += 4;
	}
}


static void v4f32_set_xyzw (float v[4], float x, float y, float z, float w)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = w;
}


static void v4f32_repeat_random (unsigned n, float r [])
{
	uint32_t const dim = 4;
	while (n--)
	{
		vf32_random (dim, r);
		r += dim;
	}
}


static void v4f32_repeat_channel (unsigned n, float r [], unsigned channel, float a)
{
	while (n--)
	{
		r [channel] = a;
		r += 4;
	}
}


static void v4f_normalize (float r [4], float v [4])
{
	vf32_normalize (4, r, v);
}



static void m4f32_mul (float y[4*4], float const a[4*4], float const b[4*4])
{
	float r[4*4] = {0.0f};
	mmf32_macc (r, a, b, 4, 4, 4);
	memcpy (y, r, sizeof (r));
}


static void mv4f32_macc (float y[4], float const a[4*4], float const b[4])
{
	mvf32_macc (y, a, b, 4, 4);
}


static void mv4f32_macc_transposed (float y[4], float const a[4*4], float const b[4])
{
	mvf32_macc_transposed (y, a, b, 4, 4);
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


void m4f32_print (float M [16], FILE * f)
{
	mf32_print (M, 4, 4, f);
}



void m4f32_set (float M [16], float c)
{
	mf32_set (M, c, 16);
}


// 4x4 matrix identity
// M := I
void m4f32_identity (float M [16])
{
	m4f32_set (M, 0);
	M [M4_S0] = 1.0f;
	M [M4_S1] = 1.0f;
	M [M4_S2] = 1.0f;
	M [M4_S3] = 1.0f;
}


void m4f32_translation (float M [16], float const t [4])
{
	m4f32_identity (M);
	M [M4_T0 + 0] = t [0];
	M [M4_T0 + 1] = t [1];
	M [M4_T0 + 2] = t [2];
	//Translate 4th dimension?
	//m [M4_T0 + 3] = t [3];
}


void m4f32_frustum
(float M [16], float l, float r, float b, float t, float n, float f)
{
	m4f32_set (M, 0);
	M [0] = (2 * n) / (r - l);
	M [5] = (2 * n) / (t - b);
	M [8] = (r + l) / (r - l);
	M [9] = (t + b) / (t - b);
	M [10] = (-f - n) / (f - n);
	M [11] = -1;
	M [14] = (-2 * f * n) / (f - n);
}


static void m4f32_perspective1 (float m[4*4], float fov, float aspect, float n, float f)
{
	float const a = 1.0f / tan (fov / 2.0f);
	//Column vector 1:
	m[0] = a / aspect;
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;
	//Column vector 2:
	m[4] = 0.0f;
	m[5] = a;
	m[6] = 0.0f;
	m[7] = 0.0f;
	//Column vector 3:
	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = -((f + n) / (f - n));
	m[11] = -1.0f;
	//Column vector 4:
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = -((2.0f * f * n) / (f - n));
	m[15] = 0.0f;
}


void m4f32_ortho
()
{

}


// 4x4 matrix inversion (glu version)
// R := M^(-1)
float m4f32_glu_inv (float R [16], float const M [16])
{
	float I [16];

	I[0] =
	M[5]  * M[10] * M[15] -
	M[5]  * M[11] * M[14] -
	M[9]  * M[6]  * M[15] +
	M[9]  * M[7]  * M[14] +
	M[13] * M[6]  * M[11] -
	M[13] * M[7]  * M[10];

	I[4] =
	-M[4]  * M[10] * M[15] +
	M[4]  * M[11] * M[14] +
	M[8]  * M[6]  * M[15] -
	M[8]  * M[7]  * M[14] -
	M[12] * M[6]  * M[11] +
	M[12] * M[7]  * M[10];

	I[8] =
	M[4]  * M[9] * M[15] -
	M[4]  * M[11] * M[13] -
	M[8]  * M[5] * M[15] +
	M[8]  * M[7] * M[13] +
	M[12] * M[5] * M[11] -
	M[12] * M[7] * M[9];

	I[12] =
	-M[4]  * M[9] * M[14] +
	M[4]  * M[10] * M[13] +
	M[8]  * M[5] * M[14] -
	M[8]  * M[6] * M[13] -
	M[12] * M[5] * M[10] +
	M[12] * M[6] * M[9];

	I[1] =
	-M[1]  * M[10] * M[15] +
	M[1]  * M[11] * M[14] +
	M[9]  * M[2] * M[15] -
	M[9]  * M[3] * M[14] -
	M[13] * M[2] * M[11] +
	M[13] * M[3] * M[10];

	I[5] =
	M[0]  * M[10] * M[15] -
	M[0]  * M[11] * M[14] -
	M[8]  * M[2] * M[15] +
	M[8]  * M[3] * M[14] +
	M[12] * M[2] * M[11] -
	M[12] * M[3] * M[10];

	I[9] =
	-M[0]  * M[9] * M[15] +
	M[0]  * M[11] * M[13] +
	M[8]  * M[1] * M[15] -
	M[8]  * M[3] * M[13] -
	M[12] * M[1] * M[11] +
	M[12] * M[3] * M[9];

	I[13] =
	M[0]  * M[9] * M[14] -
	M[0]  * M[10] * M[13] -
	M[8]  * M[1] * M[14] +
	M[8]  * M[2] * M[13] +
	M[12] * M[1] * M[10] -
	M[12] * M[2] * M[9];

	I[2] =
	M[1]  * M[6] * M[15] -
	M[1]  * M[7] * M[14] -
	M[5]  * M[2] * M[15] +
	M[5]  * M[3] * M[14] +
	M[13] * M[2] * M[7] -
	M[13] * M[3] * M[6];

	I[6] =
	-M[0]  * M[6] * M[15] +
	M[0]  * M[7] * M[14] +
	M[4]  * M[2] * M[15] -
	M[4]  * M[3] * M[14] -
	M[12] * M[2] * M[7] +
	M[12] * M[3] * M[6];

	I[10] =
	M[0]  * M[5] * M[15] -
	M[0]  * M[7] * M[13] -
	M[4]  * M[1] * M[15] +
	M[4]  * M[3] * M[13] +
	M[12] * M[1] * M[7] -
	M[12] * M[3] * M[5];

	I[14] =
	-M[0]  * M[5] * M[14] +
	M[0]  * M[6] * M[13] +
	M[4]  * M[1] * M[14] -
	M[4]  * M[2] * M[13] -
	M[12] * M[1] * M[6] +
	M[12] * M[2] * M[5];

	I[3] =
	-M[1] * M[6] * M[11] +
	M[1] * M[7] * M[10] +
	M[5] * M[2] * M[11] -
	M[5] * M[3] * M[10] -
	M[9] * M[2] * M[7] +
	M[9] * M[3] * M[6];

	I[7] =
	M[0] * M[6] * M[11] -
	M[0] * M[7] * M[10] -
	M[4] * M[2] * M[11] +
	M[4] * M[3] * M[10] +
	M[8] * M[2] * M[7] -
	M[8] * M[3] * M[6];

	I[11] =
	-M[0] * M[5] * M[11] +
	M[0] * M[7] * M[9] +
	M[4] * M[1] * M[11] -
	M[4] * M[3] * M[9] -
	M[8] * M[1] * M[7] +
	M[8] * M[3] * M[5];

	I[15] =
	M[0] * M[5] * M[10] -
	M[0] * M[6] * M[9] -
	M[4] * M[1] * M[10] +
	M[4] * M[2] * M[9] +
	M[8] * M[1] * M[6] -
	M[8] * M[2] * M[5];

	float det =
	M[0] * I[0] +
	M[1] * I[4] +
	M[2] * I[8] +
	M[3] * I[12];

	if (det == 0) {return det;}

	det = 1.0f / det;

	for (unsigned i = 0; i < 16; ++ i)
	{
		R [i] = I [i] * det;
	}

	return det;
}






void qf32_print (float q [4], FILE * f)
{
	for (size_t i = 0; i < 4; ++ i)
	{
		fprintf (f, "%f ", (double) q [i]);
	}
	fprintf (f, "\n");
}

void qf32_identity (float q [4])
{
	q [0] = 0.0f;
	q [1] = 0.0f;
	q [2] = 0.0f;
	q [3] = 1.0f;
}


float qf32_norm2 (float q [4])
{
	return v4f32_norm2 (q);
}


float qf32_norm (float const q [4])
{
	return vf32_norm (4, q);
}


void qf32_normalize (float const q [4], float r [4])
{
	v4f32_normalize (r, q);
}


void qf32_xyza (float q [4], float x, float y, float z, float a)
{
	float const c = cosf (a * 0.5f);
	float const s = sinf (a * 0.5f);
	q [0] = s * x;
	q [1] = s * y;
	q [2] = s * z;
	q [3] = c;
	//vf32_normalize (4, q, q);
	//float n = vf32_norm (4, q);
	//printf ("%f\n", n);
}


void qf32_axis_angle (float q [4], float const v [3], float angle)
{
	qf32_xyza (q, v[0], v[1], v[2], angle);
}





void qf32_mul (float r [4], float const p [4], float const q [4])
{
	float t [4];
	t [0] = p [3] * q [0] + p [0] * q [3] + p [1] * q [2] - p [2] * q [1];
	t [1] = p [3] * q [1] - p [0] * q [2] + p [1] * q [3] + p [2] * q [0];
	t [2] = p [3] * q [2] + p [0] * q [1] - p [1] * q [0] + p [2] * q [3];
	t [3] = p [3] * q [3] - p [0] * q [0] - p [1] * q [1] - p [2] * q [2];
	memcpy (r, t, sizeof (t));
}


void qf32_m4 (float R [16], float const q [4])
{
	float const l = qf32_norm (q);
	float const s = (l > 0.0f) ? (2.0f / l) : 0.0f;

	float const x = q [0];
	float const y = q [1];
	float const z = q [2];
	float const w = q [3];

	float const xx = s * x * x;
	float const xy = s * x * y;
	float const xz = s * x * z;
	float const xw = s * x * w;

	float const yy = s * y * y;
	float const yz = s * y * z;
	float const yw = s * y * w;

	float const zz = s * z * z;
	float const zw = s * z * w;

	R [0] = 1.0f - yy - zz;
	R [5] = 1.0f - xx - zz;
	R [10] = 1.0f - xx - yy;

	//column major:
	R [M4_V1 + 2] = yz + xw;
	R [M4_V2 + 0] = xz + yw;
	R [M4_V0 + 1] = xy + zw;
	R [M4_V2 + 1] = yz - xw;
	R [M4_V0 + 2] = xz - yw;
	R [M4_V1 + 0] = xy - zw;

	R [M4_V0 + 3] = 0.0f;
	R [M4_V1 + 3] = 0.0f;
	R [M4_V2 + 3] = 0.0f;

	R [M4_V3 + 0] = 0.0f;
	R [M4_V3 + 1] = 0.0f;
	R [M4_V3 + 2] = 0.0f;

	R [M4_S3] = 1.0f;
}




/*
https://gamedev.stackexchange.com/questions/28395/rotating-vector3-by-a-quaternion
void rotate_vector_by_quaternion(const Vector3& v, const Quaternion& q, Vector3& vprime)
{
	// Extract the vector part of the quaternion
	Vector3 u(q.x, q.y, q.z);

	// Extract the scalar part of the quaternion
	float s = q.w;

	// Do the math
	vprime = 2.0f * dot(u, v) * u
		  + (s*s - dot(u, u)) * v
		  + 2.0f * s * cross(u, v);
}
*/
void qf32_rotate_vector (float u[4], float y[3])
{
	float v[3];
	vf32_cpy (3, v, y);
	float uv = vvf32_dot (3, u, v);
	float ww = u[3] * u[3];
	vf32_set1 (3, y, 0.0f);
	vsf32_macc (y, u, 2.0f * uv, 3);
	vsf32_macc (y, v, ww - uv, 3);
	v3f32_crossacc_scalar (y, 2.0f * ww, u, v);
}



/*
https://github.com/datenwolf/linmath.h/blob/382ba71905c2c09f10684d19cb5a3fcadf1aba39/linmath.h#L494
Method by Fabian 'ryg' Giessen (of Farbrausch)
t = 2 * cross(q.xyz, v)
v' = v + q.w * t + cross(q.xyz, t)
*/
void qf32_rotate_vector1 (float q[4], float v[3])
{
	float r[3];
	float t[3];
	float u[3] = {q[0], q[1], q[2]};
	v3f32_cross (t, q, v);
	vsf32_mul (3, t, t, 2.0f);
	v3f32_cross (u, q, t);
	vsf32_mul (3, t, t, q[3]);
	vvf32_add (3, r, v, t);
	vvf32_add (3, r, r, u);
	vf32_cpy (3, v, r);
}


void qf32_rotate_xyza (float q[4], float x, float y, float z, float a)
{
	float u[4];
	qf32_xyza (u, x, y, z, a);
	qf32_mul (q, u, q);
}



























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







