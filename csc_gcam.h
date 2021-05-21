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
#include "csc_f32.h"






struct csc_gcam
{
	v3f32 d;//Delta
	v3f32 p;//Position
	float fov;//Field Of View
	float w;//Width
	float h;//Height
	float n;//Near
	float f;//Far
	v3f32 pyr_delta; //Euler angles
	qf32 q;//Quaternion rotation
	m4f32 mr;//Rotation matrix
	m4f32 mt;//Translation matrix
	m4f32 mp;//Projection matrix
	m4f32 mvp;//View-Projection matrix
};


void csc_gcam_init (struct csc_gcam * cam)
{
	cam->pyr_delta.x = 0.0f;
	cam->pyr_delta.y = 0.0f;
	cam->pyr_delta.z = 0.0f;
	cam->p.x = 0.0f;
	cam->p.y = 0.0f;
	cam->p.z = 0.0f;
	cam->fov = f32_deg_to_rad (45.0f);
	cam->w = 100.0f;
	cam->h = 100.0f;
	cam->n = 0.1f;
	cam->f = 10000.0f;
	qf32_identity (&cam->q);
	m4f32_identity (&cam->mp);
	m4f32_identity (&cam->mt);
	m4f32_identity (&cam->mp);
	m4f32_identity (&cam->mvp);
}


void csc_gcam_update (struct csc_gcam * cam)
{
	qf32 q_pitch;//Quaternion pitch rotation
	qf32 q_yaw;//Quaternion yaw rotation
	qf32 q_roll;//Quaternion roll rotation
	m4f32_identity (&cam->mvp);
	m4f32_identity (&cam->mt);
	m4f32_identity (&cam->mr);
	//qf32_identity (cam->q);
	//cam->pitch += cam->pitchd;
	//cam->yaw += cam->yawd;
	//cam->roll += cam->rolld;
	//qf32_ypr (cam->q, cam->yaw, cam->pitch, cam->roll);

	qf32_xyza (&q_pitch, 1.0f, 0.0f, 0.0f, cam->pyr_delta.x);
	qf32_xyza (&q_yaw,   0.0f, 1.0f, 0.0f, cam->pyr_delta.y);
	qf32_xyza (&q_roll,  0.0f, 0.0f, 1.0f, cam->pyr_delta.z);
	/*
	qf32_mul (cam->q, q_pitch, cam->q); //Apply pitch rotation
	qf32_mul (cam->q, q_roll, cam->q); //Apply roll rotation
	qf32_mul (cam->q, cam->q, q_yaw); //Apply yaw rotation
	*/
	qf32_mul (&cam->q, &q_roll, &cam->q); //Apply roll rotation
	qf32_mul (&cam->q, &q_yaw, &cam->q); //Apply yaw rotation
	qf32_mul (&cam->q, &q_pitch, &cam->q); //Apply pitch rotation
	//qf32_mul2 (&cam->q, &q_pitch, &q_yaw, &q_roll); //Apply roll rotation



	//float q[4];
	//qf32_ypr (q, cam->rolld, cam->yawd, cam->pitchd);
	//qf32_xyza (q, cam->pitchd, cam->yawd, cam->rolld, 1.0f);
	//qf32_mul (cam->q, cam->q, q);

	qf32_normalize (&cam->q, &cam->q); //Normalize quaternion against floating point error
	qf32_unit_m4 (&cam->mr, &cam->q); //Convert quaternion to rotation matrix
	v3f32_m4_mul (&cam->p, &cam->mr, &cam->d); //Accumulate the position in the direction relative to the rotation
	//mv4f32_macc_transposed3 (&cam->p, (struct m4f32[])M4F32_IDENTITY, &cam->d); //Accumulate the position in the direction relative to the rotation
	m4f32_translation3 (&cam->mt, &cam->p); //Create translation matrix
	m4f32_perspective1 (&cam->mp, cam->fov, cam->w/cam->h, cam->n, cam->f); //Create perspective matrix

	m4f32_mul (&cam->mvp, &cam->mt, &cam->mvp); //Apply translation
	m4f32_mul (&cam->mvp, &cam->mr, &cam->mvp); //Apply rotation
	m4f32_mul (&cam->mvp, &cam->mp, &cam->mvp); //Apply perspective
	//printf("\e[1;1H\e[2J");
	//mf32_print (&cam->mt, 4, 4, stdout);
	//mf32_print (&cam->mr, 4, 4, stdout);
	//mf32_print (&cam->mp, 4, 4, stdout);
}


