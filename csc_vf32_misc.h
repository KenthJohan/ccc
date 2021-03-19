/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdint.h>
#include "csc_math.h"
#include "csc_vf32.h"




static void vf32_move_center_to_zero (uint32_t dim, float const x[], uint32_t ldx, float y[], uint32_t ldy, uint32_t n, float mean[3])
{
	memset (mean, 0, sizeof (float)*dim);
	//Calculate the (mean) coordinate from (v):
	vf32_addv (dim, mean, 0, mean, 0, x, ldx, n);
	vsf32_mul (dim, mean, mean, 1.0f / (float)n);
	//Move all (v) points to origin using coordinate (mean):
	vf32_subv (dim, y, ldy, x, ldx, mean, 0, n);
}


static uint32_t vf32_amount_positive (float q[], uint32_t qn, uint32_t * qi, uint32_t count)
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


static uint32_t vf32_amount_negative (float q[], uint32_t qn, uint32_t * qi, uint32_t count)
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



/**
 * @brief Find peaks.
 * @param q[in,out] The array
 * @param qn[in]    Number of elements in \p q
 * @param g[out]    The index where the peak is located
 * @param gn[in]    Number of elements in \p g
 */
static void vf32_find_peaks (float q[], uint32_t qn, uint32_t g[], uint32_t gn, uint32_t r, uint32_t margin)
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






static void vf32_remove_low_values (float q[], uint32_t qn)
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


static uint32_t vf32_skip_zero (float q[], uint32_t qn, uint32_t * qi)
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
