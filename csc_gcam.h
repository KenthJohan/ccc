/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once
#include <SDL2/SDL.h>

#include "csc_math.h"
#include "csc_v4f32.h"
#include "csc_qf32.h"
#include "csc_m4f32.h"




struct csc_gcam
{
	v4f32 d;//Delta
	v4f32 p;//Position
	float fov;//Field Of View
	float w;//Width
	float h;//Height
	float n;//Near
	float f;//Far
	v3f32 pyrd; //Euler angles
	qf32 q;//Quaternion rotation
	m4f32 mr;//Rotation matrix
	m4f32 mt;//Translation matrix
	m4f32 mp;//Projection matrix
	m4f32 mvp;//View-Projection matrix
};


void csc_gcam_set_fov360(struct csc_gcam * cam, float angle360)
{
	cam->fov = angle360 * (M_PI/180.0f);
}


void csc_gcam_init (struct csc_gcam * cam)
{
	v3f32_set_xyz (cam->pyrd, 0.0f, 0.0f, 0.0f);
	v4f32_set_xyzw (cam->p, 0.0f, 0.0f, 0.0f, 1.0f);
	csc_gcam_set_fov360 (cam, 45.0f);
	cam->w = 100.0f;
	cam->h = 100.0f;
	cam->n = 0.1f;
	cam->f = 10000.0f;
	qf32_identity (cam->q);
	m4f32_identity (cam->mp);
	m4f32_identity (cam->mt);
	m4f32_identity (cam->mp);
	m4f32_identity (cam->mvp);
}


void csc_gcam_update (struct csc_gcam * cam)
{
	qf32 q_pitch;//Quaternion pitch rotation
	qf32 q_yaw;//Quaternion yaw rotation
	qf32 q_roll;//Quaternion roll rotation
	m4f32_identity (cam->mvp);
	m4f32_identity (cam->mt);
	//qf32_identity (cam->q);
	//cam->pitch += cam->pitchd;
	//cam->yaw += cam->yawd;
	//cam->roll += cam->rolld;
	//qf32_ypr (cam->q, cam->yaw, cam->pitch, cam->roll);

	qf32_xyza (q_pitch, 1.0f, 0.0f, 0.0f, cam->pyrd[0]);
	qf32_xyza (q_yaw,   0.0f, 1.0f, 0.0f, cam->pyrd[1]);
	qf32_xyza (q_roll,  0.0f, 0.0f, 1.0f, cam->pyrd[2]);
	/*
	qf32_mul (cam->q, q_pitch, cam->q); //Apply pitch rotation
	qf32_mul (cam->q, q_roll, cam->q); //Apply roll rotation
	qf32_mul (cam->q, cam->q, q_yaw); //Apply yaw rotation
	*/
	qf32_mul (cam->q, q_roll, cam->q); //Apply roll rotation
	qf32_mul (cam->q, q_pitch, cam->q); //Apply pitch rotation
	qf32_mul (cam->q, q_yaw, cam->q); //Apply yaw rotation


	//float q[4];
	//qf32_ypr (q, cam->rolld, cam->yawd, cam->pitchd);
	//qf32_xyza (q, cam->pitchd, cam->yawd, cam->rolld, 1.0f);
	//qf32_mul (cam->q, cam->q, q);

	qf32_normalize (cam->q, cam->q); //Normalize quaternion against floating point error
	m4f32_identity (cam->mr);
	qf32_m4 (cam->mr, cam->q); //Convert quaternion to rotation matrix
	mv4f32_macc_transposed (cam->p, cam->mr, cam->d); //Accumulate the position in the direction relative to the rotation
	m4f32_translation (cam->mt, cam->p); //Create translation matrix
	m4f32_perspective1 (cam->mp, cam->fov, cam->w/cam->h, cam->n, cam->f); //Create perspective matrix

	m4f32_mul (cam->mvp, cam->mt, cam->mvp); //Apply translation
	m4f32_mul (cam->mvp, cam->mr, cam->mvp); //Apply rotation
	m4f32_mul (cam->mvp, cam->mp, cam->mvp); //Apply perspective
	//mf32_print (view, 4, 4, stdout);
}


