/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once
//#define __USE_MINGW_ANSI_STDIO 1
#include <stddef.h> //offsetof
#include <stdint.h> //offsetof

#define UNUSED(x) (void)(x)
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
//#define INSIDE(x, a, b) ((a) <= (x) && (x) <= (b))
#define CLAMP(x, x0, x1) (((x) > (x1)) ? (x1) : (((x) < (x0)) ? (x0) : (x)))
#define SWAP(t, a, b) do{t c = (b); (b) = (a); (a) = (c);} while (0)
#define SWAPX(a, b)	((a)^=(b),(b)^=(a),(a)^=(b))
#define INRANGE(x, a, b) ((a) <= (x) && (x) <= (b))

//#define STRINGIFY0(x) #x
//#define STRINGIFY(x)  STRINGIFY0(x)
#define XSTR(a) STR(a)
#define STR(a) #a

#define container_of(ptr, type, member) ((type *)(void *)((char *)(ptr) - offsetof(type, member)))
#define container_of_const(ptr, type, member) ((type *)(void const *)((char const *)(ptr) - offsetof(type, member)))
#define countof(array) (sizeof(array) / sizeof(array[0]))







enum csc_status
{
	CSC_STATUS_COMPILED
};

enum csc_type
{
	CSC_TYPE_NONE,
	CSC_TYPE_STRING,
	CSC_TYPE_CHAR,
	CSC_TYPE_INT,
	CSC_TYPE_LONG,
	CSC_TYPE_FLOAT,
	CSC_TYPE_DOUBLE,
	CSC_TYPE_U,
	CSC_TYPE_U8 = CSC_TYPE_U+8,
	CSC_TYPE_U16 = CSC_TYPE_U+16,
	CSC_TYPE_U32 = CSC_TYPE_U+32,
	CSC_TYPE_U64 = CSC_TYPE_U+64,
	CSC_TYPE_I,
	CSC_TYPE_I8 = CSC_TYPE_I+8,
	CSC_TYPE_I16 = CSC_TYPE_I+16,
	CSC_TYPE_I32 = CSC_TYPE_I+32,
	CSC_TYPE_I64 = CSC_TYPE_I+64,
	CSC_TYPE_F32,
	CSC_TYPE_F64,
	CSC_TYPE_V4F32,
	CSC_TYPE_RGBA8888,
	CSC_TYPE_TEXTURE2D,
	CSC_TYPE_RESERVED0 = 10000,
};


union csc_union
{
	char const * val_string;
	float val_float;
	double val_double;
	int val_int;
	long val_long;
	char val_char;
	uint8_t val_u8;
	uint16_t val_u16;
	uint32_t val_u32;
	uint64_t val_u64;
	uintmax_t val_umax;
	int8_t val_i8;
	int16_t val_i16;
	int32_t val_i32;
	int64_t val_i64;
	intmax_t val_imax;
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




