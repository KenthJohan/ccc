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
	float pitch;
	float yaw;
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
}


void csc_sdlcam_build_matrix (struct csc_sdlcam * cam, float mvp[4*4])
{
	float mr[4*4];//Rotation matrix
	float mp[4*4];//Projection matrix
	float mt[4*4];//Translation matrix
	float q[4];//Quaternion rotation
	float q_pitch[4];//Quaternion pitch rotation
	float q_yaw[4];//Quaternion yaw rotation
	m4f32_identity (mvp);
	m4f32_identity (mt);
	qf32_identity (q);
	qf32_xyza (q_pitch, 1.0f, 0.0f, 0.0f, cam->pitch);
	qf32_xyza (q_yaw,   0.0f, 1.0f, 0.0f, cam->yaw);
	qf32_mul (q, q, q_pitch); //Apply pitch rotation
	qf32_mul (q, q, q_yaw); //Apply yaw rotation
	qf32_normalize (q, q); //Normalize quaternion against floating point error
	qf32_m4 (mr, q); //Convert quaternion to rotation matrix
	m4f32_perspective1 (mp, cam->fov, cam->w/cam->h, cam->n, cam->f); //Create perspective matrix
	mv4f32_macc_transposed (cam->p, mr, cam->d); //Accumulate the position in the direction relative to the rotation
	v4f32_cpy (mt + M4_VT, cam->p); //Create translation matrix (copy position into the 4th matrix column)
	m4f32_mul (mvp, mt, mvp); //Apply translation
	m4f32_mul (mvp, mr, mvp); //Apply rotation
	m4f32_mul (mvp, mp, mvp); //Apply perspective
}

