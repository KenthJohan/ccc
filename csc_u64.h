/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdint.h>

#define VU64_BITSET_ADD(x,c) ((x)[(uint64_t)(c) >> 6] |= (UINT64_C(1) << ((c) & 63)))
#define VU64_BITSET_DEL(x,c) ((x)[(uint64_t)(c) >> 6] &= ~(UINT64_C(1) << ((c) & 63)))
#define VU64_BITSET_GET(x,c) (((x)[(uint64_t)(c) >> 6] >> ((c)&63)) & 1)

static inline void vu64_bitset_from_string(uint64_t a[], char const * s)
{
	while(*s)
	{
		VU64_BITSET_ADD(a, *s);
		s++;
	}
}


//https://en.wikipedia.org/wiki/Hamming_weight
static inline int u64_hamming_weight (uint64_t x)
{
	return __builtin_popcountll (x);
}


//Calculate the size-mask or max-size:
//TODO: This might be too convulated to get the size mask:
static inline uint64_t u64_sizemask (unsigned size)
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
