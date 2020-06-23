#pragma once

#include <stdint.h>
#include "csc_math.h"
#include "csc_v3f32.h"


static void qf32_print (float q [4], FILE * f)
{
	for (size_t i = 0; i < 4; ++ i)
	{
		fprintf (f, "%f ", (double) q [i]);
	}
	fprintf (f, "\n");
}


static void qf32_identity (float q [4])
{
	q [0] = 0.0f;
	q [1] = 0.0f;
	q [2] = 0.0f;
	q [3] = 1.0f;
}


static float qf32_norm2 (float q [4])
{
	return v4f32_norm2 (q);
}


static float qf32_norm (float const q [4])
{
	return vf32_norm (4, q);
}


static void qf32_normalize (float const q [4], float r [4])
{
	v4f32_normalize (r, q);
}


static void qf32_xyza (float q [4], float x, float y, float z, float a)
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


static void qf32_axis_angle (float q [4], float const v [3], float angle)
{
	qf32_xyza (q, v[0], v[1], v[2], angle);
}





static void qf32_mul (float r [4], float const p [4], float const q [4])
{
	float t [4];
	t [0] = p [3] * q [0] + p [0] * q [3] + p [1] * q [2] - p [2] * q [1];
	t [1] = p [3] * q [1] - p [0] * q [2] + p [1] * q [3] + p [2] * q [0];
	t [2] = p [3] * q [2] + p [0] * q [1] - p [1] * q [0] + p [2] * q [3];
	t [3] = p [3] * q [3] - p [0] * q [0] - p [1] * q [1] - p [2] * q [2];
	memcpy (r, t, sizeof (t));
}


static void qf32_m4 (float R [16], float const q [4])
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

	/*
	R [M4_V0 + 3] = 0.0f;
	R [M4_V1 + 3] = 0.0f;
	R [M4_V2 + 3] = 0.0f;

	R [M4_V3 + 0] = 0.0f;
	R [M4_V3 + 1] = 0.0f;
	R [M4_V3 + 2] = 0.0f;

	R [M4_S3] = 1.0f;
	*/
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
static void qf32_rotate_vector_fixthis (float u[4], float y[3])
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
static void qf32_rotate_vector (float const q[4], float const v[3], float r[3])
{
	ASSERT (v != r);
	float t[3];
	float u[3] = {q[0], q[1], q[2]};
	v3f32_cross (t, q, v);
	vsf32_mul (3, t, t, 2.0f);
	v3f32_cross (u, q, t);
	vsf32_mul (3, t, t, q[3]);
	vvf32_add (3, r, v, t);
	vvf32_add (3, r, r, u);
}


static void qf32_rotate_vector1 (float q[4], float v[3])
{
	float r[3];
	qf32_rotate_vector (q, v, r);
	vf32_cpy (3, v, r);
}


static void qf32_rotate_vector_array (float q[4], float v[], unsigned n)
{
	while (n--)
	{
		qf32_rotate_vector1 (q, v);
		v += 4;
	}
}


static void qf32_rotate_xyza (float q[4], float x, float y, float z, float a)
{
	float u[4];
	qf32_xyza (u, x, y, z, a);
	qf32_mul (q, u, q);
}



static void qf32_ypr (float q[4], float yaw, float pitch, float roll) // yaw (Z), pitch (Y), roll (X)
{
	// Abbreviations for the various angular functions
	float cy = cos(yaw * 0.5);
	float sy = sin(yaw * 0.5);
	float cp = cos(pitch * 0.5);
	float sp = sin(pitch * 0.5);
	float cr = cos(roll * 0.5);
	float sr = sin(roll * 0.5);
	q[0] = sr * cp * cy - cr * sp * sy;
	q[1] = cr * sp * cy + sr * cp * sy;
	q[2] = cr * cp * sy - sr * sp * cy;
	q[3] = cr * cp * cy + sr * sp * sy;
}

