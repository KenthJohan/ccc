/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once
#include <stddef.h> //offsetof
#include <stdint.h> //offsetof



#define CSC_RGB676_INDEX(R,G,B) ((42*(R))+(6*G)+(B))
#define CSC_RGB676_RMAX 5
#define CSC_RGB676_GMAX 6
#define CSC_RGB676_BMAX 5


#define CSC_RGB884_INDEX(R,G,B) (((R) << 5) | ((G) << 2) | (B))
#define CSC_RGB884_RMAX 7
#define CSC_RGB884_GMAX 7
#define CSC_RGB884_BMAX 3

struct csc_f32rgba
{
	float r;
	float g;
	float b;
	float a;
};


struct csc_u8rgba
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};
