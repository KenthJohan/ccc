#pragma once
#include <SDL2/SDL.h>

#include "csc_math.h"



struct csc_sdlcam
{
	float d[4];//Delta
	float p[4];//Position
	float fov;//Field Of View
	float w;//Width
	float h;//Height
	float n;//Near
	float f;//Far

	float pitchd;
	float pitch;
	float yawd;
	float yaw;
	float rolld;
	float roll;


	float q[4];//Quaternion rotation

	float mr[4*4];//Rotation matrix
	float mt[4*4];//Translation matrix
	float mp[4*4];//Projection matrix
	float mvp[4*4];//View-Projection matrix
};


void csc_sdlcam_init (struct csc_sdlcam * cam)
{
	v4f32_set_xyzw (cam->p, 0.0f, 0.0f, 0.0f, 1.0f);
	cam->fov = 45.0f*(M_PI/180.0f);
	cam->w = 100.0f;
	cam->h = 100.0f;
	cam->n = 0.1f;
	cam->f = 10000.0f;
	cam->pitch = 0;
	cam->yaw = 0;
	cam->roll = 0;
	qf32_identity (cam->q);
}


void csc_sdlcam_build (struct csc_sdlcam * cam)
{
	float q_pitch[4];//Quaternion pitch rotation
	float q_yaw[4];//Quaternion yaw rotation
	float q_roll[4];//Quaternion roll rotation
	m4f32_identity (cam->mvp);
	m4f32_identity (cam->mt);
	//qf32_identity (cam->q);
	//cam->pitch += cam->pitchd;
	//cam->yaw += cam->yawd;
	//cam->roll += cam->rolld;
	//qf32_ypr (cam->q, cam->yaw, cam->pitch, cam->roll);

	qf32_xyza (q_pitch, 1.0f, 0.0f, 0.0f, cam->pitchd);
	qf32_xyza (q_yaw,   0.0f, 1.0f, 0.0f, cam->yawd);
	qf32_xyza (q_roll,  0.0f, 0.0f, 1.0f, cam->rolld);
	qf32_mul (cam->q, q_pitch, cam->q); //Apply pitch rotation
	qf32_mul (cam->q, q_roll, cam->q); //Apply roll rotation
	qf32_mul (cam->q, cam->q, q_yaw); //Apply yaw rotation




	//float q[4];
	//qf32_ypr (q, cam->rolld, cam->yawd, cam->pitchd);
	//qf32_xyza (q, cam->pitchd, cam->yawd, cam->rolld, 1.0f);
	//qf32_mul (cam->q, cam->q, q);

	qf32_normalize (cam->q, cam->q); //Normalize quaternion against floating point error
	m4f32_identity (cam->mr);
	qf32_m4 (cam->mr, cam->q); //Convert quaternion to rotation matrix
	mv4f32_macc_transposed (cam->p, cam->mr, cam->d); //Accumulate the position in the direction relative to the rotation
	v4f32_cpy (cam->mt + M4_VT, cam->p); //Create translation matrix (copy position into the 4th matrix column)
	m4f32_perspective1 (cam->mp, cam->fov, cam->w/cam->h, cam->n, cam->f); //Create perspective matrix
	m4f32_mul (cam->mvp, cam->mt, cam->mvp); //Apply translation
	m4f32_mul (cam->mvp, cam->mr, cam->mvp); //Apply rotation
	m4f32_mul (cam->mvp, cam->mp, cam->mvp); //Apply perspective
	//mf32_print (view, 4, 4, stdout);
}


