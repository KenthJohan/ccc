/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdint.h>



//If a maps to x, then b maps from x
static inline void
vu32_map (uint32_t a [], uint32_t b [], uint32_t n)
{
	for (uint32_t i = 0; i < n; ++i)
	{
		b [a [i]] = i;
	}
}


//Calculate the size-mask or max-size:
//TODO: This might be too convulated to get the size mask:
static inline uint64_t
u64_sizemask (unsigned size)
{
	uint64_t mask;
	if (size == 64)
	{
		mask = UINT64_MAX;
	}
	else
	{
		mask = ((UINT64_C (1) << (uint64_t)size) - UINT64_C (1));
	}
	return mask;
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

//https://en.wikipedia.org/wiki/Hamming_weight
static int u64_hamming_weight (uint64_t x)
{
	return __builtin_popcountll (x);
}

