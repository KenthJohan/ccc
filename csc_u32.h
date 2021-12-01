/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdint.h>


#define U32_U8_LE(a, b, c, d) ((uint32_t)((a) | (b) << 8 | (c) << 16 | (d) << 24))
#define U32_U8_BE(d, c, b, a) ((uint32_t)((a) | (b) << 8 | (c) << 16 | (d) << 24))
#define U32_V4U8_LE(s) U32_U8_LE(s[0],s[1],s[2],s[3])
#define U32_V4U8_BE(s) U32_U8_BE(s[0],s[1],s[2],s[3])



//If a maps to x, then b maps from x
static inline void
vu32_map (uint32_t a [], uint32_t b [], uint32_t n)
{
	for (uint32_t i = 0; i < n; ++i)
	{
		b [a [i]] = i;
	}
}






//https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
// compute the next highest power of 2 of 32-bit v
static uint32_t u32_roundup_pow2(uint32_t v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v;
}



//https://en.wikipedia.org/wiki/Hamming_weight
static int u32_hamming_weight (uint32_t x)
{
	return __builtin_popcount (x);
}


