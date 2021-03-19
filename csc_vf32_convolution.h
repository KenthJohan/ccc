/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdint.h>
#include "csc_math.h"
#include "csc_vf32.h"


static void vf32_convolution1d (float const q[], uint32_t n, float u[], float k[], uint32_t kn)
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
