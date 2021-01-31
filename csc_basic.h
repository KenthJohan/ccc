/* SPDX-License-Identifier: GPL-2.0 */
#pragma once
#include <stddef.h> //offsetof
#include <stdint.h> //offsetof


#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
//#define INSIDE(x, a, b) ((a) <= (x) && (x) <= (b))
#define CLAMP(x, x0, x1) (((x) > (x1)) ? (x1) : (((x) < (x0)) ? (x0) : (x)))
#define SWAP(t, a, b) do{t c = (b); (b) = (a); (a) = (c);} while (0)
#define SWAPX(a, b)	((a)^=(b),(b)^=(a),(a)^=(b))
#define INRANGE(x, a, b) ((a) <= (x) && (x) <= (b))
#define U8_U32_LE(a, b, c, d) ((uint32_t)((a) | (b) << 8 | (c) << 16 | (d) << 24))
#define U8_U32_BE(d, c, b, a) ((uint32_t)((a) | (b) << 8 | (c) << 16 | (d) << 24))
#define STR4_U32_LE(s) U8_U32_LE(s[0],s[1],s[2],s[3])
#define STR4_U32_BE(s) U8_U32_BE(s[0],s[1],s[2],s[3])
#define STRINGIFY0(x) #x
#define STRINGIFY(x)  STRINGIFY0(x)

#define container_of(ptr, type, member) ((type *)(void *)((char *)(ptr) - offsetof(type, member)))
#define container_of_const(ptr, type, member) ((type *)(void const *)((char const *)(ptr) - offsetof(type, member)))

#define countof(array) (sizeof(array) / sizeof(array[0]))

#define BITS_SUBSET(a,b) ((a) & (b))

enum csc_type
{
	CSC_TYPE_U8,
	CSC_TYPE_U16,
	CSC_TYPE_U32,
	CSC_TYPE_U64,
	CSC_TYPE_I8,
	CSC_TYPE_I16,
	CSC_TYPE_I32,
	CSC_TYPE_I64,
	CSC_TYPE_F32,
	CSC_TYPE_F64,


	CSC_TYPE_V4F32,


	CSC_TYPE_RGBA8888,

	CSC_TYPE_TEXTURE2D,
};



/*

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

typedef uint32_t c32; //character
typedef uint16_t c16; //character
typedef uint8_t c8; //character

typedef uint8_t utf8; //utf character
typedef uint16_t utf16; //utf character
typedef uint32_t utf32; //utf character
*/


//If a maps to x, then b maps from x
static inline void
csc_inverse_121 (uint32_t a [], uint32_t b [], uint32_t n)
{
	for (uint32_t i = 0; i < n; ++i)
	{
		b [a [i]] = i;
	}
}


//Calculate the size-mask or max-size:
//TODO: This might be too convulated to get the size mask:
static inline uint64_t
csc_sizemask64 (unsigned size)
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
unsigned int csc_round_up_pow2(unsigned int v)
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




void csc_mask_unravel_u64 (uint64_t mask, uint64_t set[64])
{
	set[0] = mask & 0x0000000000000001;
	set[1] = mask & 0x0000000000000002;
	set[2] = mask & 0x0000000000000004;
	set[3] = mask & 0x0000000000000008;
	set[4] = mask & 0x0000000000000010;
	set[5] = mask & 0x0000000000000020;
	//TODO: etc...
}


//https://en.wikipedia.org/wiki/Hamming_weight
int csc_hamming_weight_u32 (uint32_t x)
{
	return __builtin_popcount (x);
}

//https://en.wikipedia.org/wiki/Hamming_weight
int csc_hamming_weight_u64 (uint32_t x)
{
	return __builtin_popcountll (x);
}




