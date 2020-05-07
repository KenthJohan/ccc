/*
MIT License

Copyright (c) 2019 CSC Project

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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


#define container_of(ptr, type, member) ((type *)(void *)((char *)(ptr) - offsetof(type, member)))
#define container_of_const(ptr, type, member) ((type *)(void const *)((char const *)(ptr) - offsetof(type, member)))

#define countof(array) (sizeof(array) / sizeof(array[0]))

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



