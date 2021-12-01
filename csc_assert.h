/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/

//https://github.com/zephyrproject-rtos/zephyr/blob/main/subsys/testsuite/ztest/include/ztest_assert.h
//https://github.com/libuv/libuv/blob/6564ccc90073296769ac17319bba3f3357005603/test/task.h
//https://gcc.gnu.org/onlinedocs/gcc/Variadic-Macros.html
//http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2153.htm

#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>

#define ASSERT_COLOR "\e[38;2;220;60;60m"
#define ASSERT_COLOR1 "\e[38;2;80;190;80m"
#define ASSERT_COLOR2 "\e[38;2;190;80;80m"
#define ASSERT_COLOR_FILENAME "\e[38;2;130;130;130m"
#define ASSERT_COLOR_FUNCNAME "\e[38;2;100;100;140m"
#define ASSERT_COLOR_EXP "\e[38;2;150;150;200m"

#define ASSERT_COMPARE(a, o, b, t, p) \
	do{if(!((t)(a) o (t)(b))){assert_format(__COUNTER__, __FILE__, __LINE__, __func__, NULL, \
	"(" ASSERT_COLOR1 "%s\e[0m %s " ASSERT_COLOR2 "%s\e[0m) eval (" \
	ASSERT_COLOR1 "%" p "\e[0m " #o ASSERT_COLOR2" %" p "\e[0m)", #a, #o, #b, (t)(a), (t)(b) \
	);}} while(0)


// Generic assertion
#define ASSERT(A)               do{if(!(A)){assert_format(__COUNTER__, __FILE__, __LINE__, __func__, #A, (NULL)             );}}while(0)
#define ASSERTF(A, F, ...)      do{if(!(A)){assert_format(__COUNTER__, __FILE__, __LINE__, __func__, #A, (F), ## __VA_ARGS__);}}while(0)

// Comparison assertion
// Can be used for any unsigned
#define ASSERT_EQ_U(a,b) ASSERT_COMPARE(a, ==, b, uintmax_t, PRIuMAX)
#define ASSERT_NE_U(a,b) ASSERT_COMPARE(a, !=, b, uintmax_t, PRIuMAX)
#define ASSERT_GT_U(a,b) ASSERT_COMPARE(a, > , b, uintmax_t, PRIuMAX)
#define ASSERT_GE_U(a,b) ASSERT_COMPARE(a, >=, b, uintmax_t, PRIuMAX)
#define ASSERT_LT_U(a,b) ASSERT_COMPARE(a, < , b, uintmax_t, PRIuMAX)
#define ASSERT_LE_U(a,b) ASSERT_COMPARE(a, <=, b, uintmax_t, PRIuMAX)

// Comparison assertion
// Can be used for any signed
#define ASSERT_EQ_I(a,b) ASSERT_COMPARE(a, ==, b, intmax_t, PRIiMAX)
#define ASSERT_NE_I(a,b) ASSERT_COMPARE(a, !=, b, intmax_t, PRIiMAX)
#define ASSERT_GT_I(a,b) ASSERT_COMPARE(a, > , b, intmax_t, PRIiMAX)
#define ASSERT_GE_I(a,b) ASSERT_COMPARE(a, >=, b, intmax_t, PRIiMAX)
#define ASSERT_LT_I(a,b) ASSERT_COMPARE(a, < , b, intmax_t, PRIiMAX)
#define ASSERT_LE_I(a,b) ASSERT_COMPARE(a, <=, b, intmax_t, PRIiMAX)

// Comparison assertion
// Can be used for only float or double
#define ASSERT_EQ_F(a,b) ASSERT_COMPARE(a, ==, b, double, "f")
#define ASSERT_NE_F(a,b) ASSERT_COMPARE(a, !=, b, double, "f")
#define ASSERT_GT_F(a,b) ASSERT_COMPARE(a, > , b, double, "f")
#define ASSERT_GE_F(a,b) ASSERT_COMPARE(a, >=, b, double, "f")
#define ASSERT_LT_F(a,b) ASSERT_COMPARE(a, < , b, double, "f")
#define ASSERT_LE_F(a,b) ASSERT_COMPARE(a, <=, b, double, "f")


static void assert_format_va
(int id, char const * file, int line, char const * fn, char const * exp, char const * fmt, va_list va)
{
	fprintf (stderr, ASSERT_COLOR "A%i " ASSERT_COLOR_FILENAME "%s:%i\e[0m " ASSERT_COLOR_FUNCNAME "%s()\e[0m: ", id, file, line, fn);

	if (errno != 0)
	{
		fprintf (stderr, "errno(%i): " ASSERT_COLOR "%s\e[0m. ", errno, strerror (errno));
	}
	if (exp)
	{
		fprintf (stderr, "exp: "ASSERT_COLOR_EXP "%s\e[0m. ", exp);
	}
	if (fmt)
	{
		vfprintf (stderr, fmt, va);
	}
	fprintf (stderr, "\n");
	fflush (stderr);
}


__attribute__ ((noreturn))
__attribute__ ((format (printf, 6, 7)))
static void assert_format 
(int id,char const * file,int line,char const * fn,char const * exp,char const * fmt,...)
{
	va_list va;
	va_start (va, fmt);
	assert_format_va (id, file, line, fn, exp, fmt, va);
	va_end (va);
	exit (1);
}










