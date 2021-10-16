/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "csc_basic.h"
#include "csc_assert.h"

#define CSC_STRNCMP_LITERAL(str1,str2) strncmp ((str1), (str2), sizeof ((str2))-1)


#define STR_INB(x, a, b, base) INRANGE ((x), (a), MIN ((base) + (a), (b)))
#define STR_SIGNED (1 << 0)
#define STR_UNSIGNED (1 << 1)
#define STR_ALIGN_LEFT (1 << 2)
#define STR_ALIGN_RIGHT (1 << 3)
#define STR_ALIGN_MIDDLE (1 << 4)






static inline void str_rev (char * o, uint32_t n)
{
	char * e = o + n;
	n /= 2;
	while (n--)
	{
		e --;
		SWAPX (*o, *e);
		o ++;
	}
}



static inline void str_rep (char * o, uint32_t n, char pad)
{
	while (n--)
	{
		*o = pad;
		o ++;
	}
}




/*
returns positive when (str1) contains substring of (str2) seperated by (delimiters).
returns 0 otherwise.
eg:
	char * ext = strrchr (filename, '.');
	size_t n = csc_str_contains1 (ext, ".txt, .png ,.json", ", ");
*/
static inline size_t csc_str_contains1
(char const * str1, char const * str2, char const * delimiters)
{
	if (str1 == NULL) {return 0;}
	if (str2 == NULL) {return 0;}
	size_t n = 0;
	while (1)
	{
		str2 += strspn (str2, delimiters);
		int end = ((*str1) == '\0') || ((*str2) == '\0');
		if (end) {break;}
		else if ((*str1) == (*str2))
		{
			str1 ++;
			str2 ++;
			n ++;
		}
		else
		{
			str1 -= n;
			n = 0;
			str2 += strcspn (str2, delimiters);
		}
	}
	return n;
}

/*
eg:
	char * ext1 = strrchr ("C:/docs/hello.txt", '.');
	char * ext0 = strrchr ("C:/docs/hello.txt", '/');
*/
static inline int csc_str_next_cmp
(char const ** p, int * col, char const * str)
{
	size_t l = strlen (str);
	int diff = strncmp (*p, str, l);
	if (diff == 0)
	{
		(*p) += l;
		(*col) += l;
		return (int) l;
	}
	return 0;
}



static inline void csc_str_skip (char const ** p, int (*f)(int))
{
	while (f (**p)) {(*p)++;}
}


static inline int csc_isindentifer (int a)
{
	return isalnum (a) || (a == '_');
}



static inline int csc_next_indentifer (char const ** p, int * col)
{
	ASSERT_PARAM_NOTNULL (p);
	ASSERT_PARAM_NOTNULL (col);
	char const * q = (*p);
	csc_str_skip (p, csc_isindentifer);
	ptrdiff_t n = (*p) - q;
	(*col) += n;
	return (int)n;
}



static inline int csc_next_literal (char const ** p, int * col)
{
	ASSERT_PARAM_NOTNULL (p);
	ASSERT_PARAM_NOTNULL (col);
	char const * q = (*p);
	csc_str_skip (p, isdigit);
	ptrdiff_t n = (*p) - q;
	(*col) += n;
	return (int)n;
}



static inline void csc_str_print_hex_array
(char * s, size_t sn, uint8_t * v, size_t vn, char const * format, size_t step)
{
	ASSERT_PARAM_NOTNULL (s);
	ASSERT_PARAM_NOTNULL (v);
	ASSERT_PARAM_NOTNULL (format);
	while (vn--)
	{
		if (sn < step) {break;}
		s += snprintf (s, step, format, *v);
		v ++;
		sn -= step;
	}
}



static inline void str_skip_space (char ** p)
{
	ASSERT_PARAM_NOTNULL (p);
	while (isspace (**p))
	{
		(*p)++;
	}
}



static inline void str_skip_alnum (char ** p)
{
	ASSERT_PARAM_NOTNULL (p);
	while (isalnum (**p))
	{
		(*p)++;
	}
}



static inline void str_skip_until (char const ** p, char * needles)
{
	ASSERT_PARAM_NOTNULL (p);
	ASSERT_PARAM_NOTNULL (needles);
	while ((**p) && strchr (needles, **p) == NULL)
	{
		(*p)++;
	}
}



static inline void str_skip_after (char const ** p, char * needles)
{
	ASSERT_PARAM_NOTNULL (p);
	ASSERT_PARAM_NOTNULL (needles);
	while ((**p) && strchr (needles, **p) != NULL)
	{
		(*p)++;
	}
}


/**
 * @brief csc_cmp_str Compares two strings
 * @param s1 First string
 * @param s2 Second string
 * @param s1_end End of first string. Can be NULL.
 * @param s2_end End of second string. Can be NULL.
 * @return
 */
static inline int csc_str_cmp
(char const * s1, char const * s2, char const * s1_end, char const * s2_end)
{
	ASSERT_PARAM_NOTNULL (s1);
	ASSERT_PARAM_NOTNULL (s2);
	int d = 0;
	while (1)
	{
		if (s1_end && (s1 >= s1_end)) {break;}
		if (s2_end && (s2 >= s2_end)) {break;}
		d = s1[0] - s2[0];
		if (d) {break;}
		if (s1[0] == '\0') {break;}
		if (s2[0] == '\0') {break;}
		s1++;
		s2++;
	}
	return d;
}
