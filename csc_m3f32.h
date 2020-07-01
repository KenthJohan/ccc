#pragma once

#include <stdint.h>
#include "csc_math.h"

void m3f32_print (float M [3*3], FILE * f)
{
	mf32_print (M, 3, 3, f);
}





/**
 * @brief Perfoms (xx^t) which outputs a symmetric matrix
 * @param m
 * @param x
 * @param x_stride
 * @param x_count
 */
void m3f32_symmetric_xxt (uint32_t dim, float m[], uint32_t m_stride, float x[], uint32_t x_stride, uint32_t x_count)
{
	for (uint32_t i = 0; i < x_count; ++i)
	{
		for (uint32_t j = 0; j < dim; ++j)
		{
			vsf32_macc (m + m_stride*j, x, x[j], dim);
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
