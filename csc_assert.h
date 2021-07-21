/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "csc_tcol.h"


//https://gcc.gnu.org/onlinedocs/gcc/Variadic-Macros.html
//http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2153.htm
#define ASSERT_CARGS __COUNTER__, __FILE__, __LINE__, __func__

#define ASSERT_STYLE_EXP        TFG(255,200,255) "%s"              TCOL_RST
#define ASSERT_STYLE_VARNAME    TFG(123,200,255) "%s"              TCOL_RST
#define ASSERT_STYLE_I          TFG(123,123,255) "%j"             TCOL_RST
#define ASSERT_STYLE_U          TFG(123,123,255) "%ju"             TCOL_RST
#define ASSERT_STYLE_P          TFG(123,123,255) "%p"              TCOL_RST
#define ASSERT_STYLE_NULL       TFG(123,123,255) "NULL"            TCOL_RST
#define ASSERT_STYLE_FALSE      TFG(123,123,255) "FALSE"           TCOL_RST
#define ASSERT_STYLE_TRUE       TFG(123,123,255) "TRUE"            TCOL_RST
#define ASSERT_STYLE_ASSERT     TFG(255,66 ,66)  "A%04x"           TCOL_RST
#define ASSERT_STYLE_FILENAME   TFG(99 ,99 ,99)  "%s"              TCOL_RST
#define ASSERT_STYLE_LINE       TFG(99 ,99 ,99)  "%i"              TCOL_RST
#define ASSERT_STYLE_FNAME      TFG(99 ,120,99)  "%s"              TCOL_RST
#define ASSERT_STYLE_ERRNO      TFG(190,90 ,90)  "%s (errno=%i)"   TCOL_RST

#define ASSERT_STYLE_NOTNULL         "Failed assertion: (" ASSERT_STYLE_VARNAME " /= "  ASSERT_STYLE_NULL    ") evaluated as (" ASSERT_STYLE_P " /= " ASSERT_STYLE_NULL ")"
#define ASSERT_STYLE_PARAM_NOTNULL   "Failed assertion: Paramater (" ASSERT_STYLE_VARNAME " /= "  ASSERT_STYLE_NULL    ") evaluated as (" ASSERT_STYLE_P " /= " ASSERT_STYLE_NULL ")"
#define ASSERT_STYLE_ISFALSE         "Failed assertion: (" ASSERT_STYLE_VARNAME " = "   ASSERT_STYLE_FALSE    ") evaluated as (" ASSERT_STYLE_P " = " ASSERT_STYLE_FALSE ")"
#define ASSERT_STYLE_ISTRUE          "Failed assertion: (" ASSERT_STYLE_VARNAME " = "   ASSERT_STYLE_TRUE     ") evaluated as (" ASSERT_STYLE_P " = " ASSERT_STYLE_TRUE ")"
#define ASSERT_STYLE_ISPOW2          "Failed assertion: (" ASSERT_STYLE_VARNAME " = "   ASSERT_STYLE_U ") is a positive integer power of two."
#define ASSERT_STYLE_ISALIGN         "Failed assertion: (" ASSERT_STYLE_VARNAME " = "   ASSERT_STYLE_P") aligned to ("ASSERT_STYLE_VARNAME" = "ASSERT_STYLE_U"), "ASSERT_STYLE_U
#define ASSERT_STYLE_LTI             "Failed assertion: (" ASSERT_STYLE_VARNAME " < "   ASSERT_STYLE_VARNAME ") evaluated as (" ASSERT_STYLE_I " < "  ASSERT_STYLE_I	")"
#define ASSERT_STYLE_LTU             "Failed assertion: (" ASSERT_STYLE_VARNAME " < "   ASSERT_STYLE_VARNAME ") evaluated as (" ASSERT_STYLE_U " < "  ASSERT_STYLE_U	")"
#define ASSERT_STYLE_LTP             "Failed assertion: (" ASSERT_STYLE_VARNAME " < "   ASSERT_STYLE_VARNAME ") evaluated as (" ASSERT_STYLE_P " < "  ASSERT_STYLE_P    ")"
#define ASSERT_STYLE_LTEI            "Failed assertion: (" ASSERT_STYLE_VARNAME " < "   ASSERT_STYLE_VARNAME ") evaluated as (" ASSERT_STYLE_I " < "  ASSERT_STYLE_I	")"
#define ASSERT_STYLE_LTEU            "Failed assertion: (" ASSERT_STYLE_VARNAME " < "   ASSERT_STYLE_VARNAME ") evaluated as (" ASSERT_STYLE_U " < "  ASSERT_STYLE_U	")"
#define ASSERT_STYLE_LTEP            "Failed assertion: (" ASSERT_STYLE_VARNAME " < "   ASSERT_STYLE_VARNAME ") evaluated as (" ASSERT_STYLE_P " < "  ASSERT_STYLE_P    ")"
#define ASSERT_STYLE_GTI             "Failed assertion: (" ASSERT_STYLE_VARNAME " > "   ASSERT_STYLE_VARNAME ") evaluated as (" ASSERT_STYLE_I " > "  ASSERT_STYLE_I    ")"
#define ASSERT_STYLE_GTU             "Failed assertion: (" ASSERT_STYLE_VARNAME " > "   ASSERT_STYLE_VARNAME ") evaluated as (" ASSERT_STYLE_U " > "  ASSERT_STYLE_U    ")"
#define ASSERT_STYLE_GTP             "Failed assertion: (" ASSERT_STYLE_VARNAME " > "   ASSERT_STYLE_VARNAME ") evaluated as (" ASSERT_STYLE_P " > "  ASSERT_STYLE_P    ")"
#define ASSERT_STYLE_GTEI            "Failed assertion: (" ASSERT_STYLE_VARNAME " > "   ASSERT_STYLE_VARNAME ") evaluated as (" ASSERT_STYLE_I " > "  ASSERT_STYLE_I    ")"
#define ASSERT_STYLE_GTEU            "Failed assertion: (" ASSERT_STYLE_VARNAME " > "   ASSERT_STYLE_VARNAME ") evaluated as (" ASSERT_STYLE_U " > "  ASSERT_STYLE_U    ")"
#define ASSERT_STYLE_GTEP            "Failed assertion: (" ASSERT_STYLE_VARNAME " > "   ASSERT_STYLE_VARNAME ") evaluated as (" ASSERT_STYLE_P " > "  ASSERT_STYLE_P    ")"
#define ASSERT_STYLE_EQI             "Failed assertion: (" ASSERT_STYLE_VARNAME " = "   ASSERT_STYLE_VARNAME ") evaluated as (" ASSERT_STYLE_I " = "  ASSERT_STYLE_I    ")"
#define ASSERT_STYLE_EQU             "Failed assertion: (" ASSERT_STYLE_VARNAME " = "   ASSERT_STYLE_VARNAME ") evaluated as (" ASSERT_STYLE_U " = "  ASSERT_STYLE_U    ")"
#define ASSERT_STYLE_EQP             "Failed assertion: (" ASSERT_STYLE_VARNAME " = "   ASSERT_STYLE_VARNAME ") evaluated as (" ASSERT_STYLE_P " = "  ASSERT_STYLE_P    ")"
#define ASSERT_STYLE_NEQI            "Failed assertion: (" ASSERT_STYLE_VARNAME " /= "  ASSERT_STYLE_VARNAME ") evaluated as (" ASSERT_STYLE_I " /= "  ASSERT_STYLE_I   ")"
#define ASSERT_STYLE_NEQU            "Failed assertion: (" ASSERT_STYLE_VARNAME " /= "  ASSERT_STYLE_VARNAME ") evaluated as (" ASSERT_STYLE_U " /= "  ASSERT_STYLE_U   ")"
#define ASSERT_STYLE_NEQP            "Failed assertion: (" ASSERT_STYLE_VARNAME " /= "  ASSERT_STYLE_VARNAME ") evaluated as (" ASSERT_STYLE_P " /= "  ASSERT_STYLE_P   ")"

#define ASSERT(A)               do{if(!(A)){assert_format(ASSERT_CARGS, #A, (NULL)             );}}while(0)
#define ASSERTF(A, F, ...)      do{if(!(A)){assert_format(ASSERT_CARGS, #A, (F), ## __VA_ARGS__);}}while(0)
#define ASSERT_NOTNULL(A)       do{if(!(A))       {assert_format(ASSERT_CARGS, NULL, ASSERT_STYLE_NOTNULL,        #A, (A));}}while(0)
#define ASSERT_PARAM_NOTNULL(A) do{if(!(A))       {assert_format(ASSERT_CARGS, NULL, ASSERT_STYLE_PARAM_NOTNULL,  #A, (A));}}while(0)
#define ASSERT_LTI(A,B)         do{if((A)>=(B))   {assert_format(ASSERT_CARGS, NULL, ASSERT_STYLE_LTI,            #A, #B,  (intmax_t)(A),  (intmax_t)(B));}}while(0)
#define ASSERT_LTU(A,B)         do{if((A)>=(B))   {assert_format(ASSERT_CARGS, NULL, ASSERT_STYLE_LTU,            #A, #B, (uintmax_t)(A), (uintmax_t)(B));}}while(0)
#define ASSERT_LTP(A,B)         do{if((A)>=(B))   {assert_format(ASSERT_CARGS, NULL, ASSERT_STYLE_LTP,            #A, #B,            (A),            (B));}}while(0)
#define ASSERT_LTEI(A,B)        do{if((A)>(B))    {assert_format(ASSERT_CARGS, NULL, ASSERT_STYLE_LETI,           #A, #B,  (intmax_t)(A),  (intmax_t)(B));}}while(0)
#define ASSERT_LTEU(A,B)        do{if((A)>(B))    {assert_format(ASSERT_CARGS, NULL, ASSERT_STYLE_LETU,           #A, #B, (uintmax_t)(A), (uintmax_t)(B));}}while(0)
#define ASSERT_LTEP(A,B)        do{if((A)>(B))    {assert_format(ASSERT_CARGS, NULL, ASSERT_STYLE_LETP,           #A, #B,            (A),            (B));}}while(0)
#define ASSERT_GTI(A,B)         do{if((A)<=(B))   {assert_format(ASSERT_CARGS, NULL, ASSERT_STYLE_GTI,            #A, #B,  (intmax_t)(A),  (intmax_t)(B));}}while(0)
#define ASSERT_GTU(A,B)         do{if((A)<=(B))   {assert_format(ASSERT_CARGS, NULL, ASSERT_STYLE_GTU,            #A, #B, (uintmax_t)(A), (uintmax_t)(B));}}while(0)
#define ASSERT_GTP(A,B)         do{if((A)<=(B))   {assert_format(ASSERT_CARGS, NULL, ASSERT_STYLE_GTP,            #A, #B,            (A),            (B));}}while(0)
#define ASSERT_GTEI(A,B)        do{if((A)<(B))    {assert_format(ASSERT_CARGS, NULL, ASSERT_STYLE_GTEI,           #A, #B,  (intmax_t)(A),  (intmax_t)(B));}}while(0)
#define ASSERT_GTEU(A,B)        do{if((A)<(B))    {assert_format(ASSERT_CARGS, NULL, ASSERT_STYLE_GTEU,           #A, #B, (uintmax_t)(A), (uintmax_t)(B));}}while(0)
#define ASSERT_GTEP(A,B)        do{if((A)<(B))    {assert_format(ASSERT_CARGS, NULL, ASSERT_STYLE_GTEP,           #A, #B,            (A),            (B));}}while(0)
#define ASSERT_EQI(A,B)         do{if((A)!=(B))   {assert_format(ASSERT_CARGS, NULL, ASSERT_STYLE_EQI,            #A, #B,  (intmax_t)(A),  (intmax_t)(B));}}while(0)
#define ASSERT_EQU(A,B)         do{if((A)!=(B))   {assert_format(ASSERT_CARGS, NULL, ASSERT_STYLE_EQU,            #A, #B, (uintmax_t)(A), (uintmax_t)(B));}}while(0)
#define ASSERT_EQP(A,B)         do{if((A)!=(B))   {assert_format(ASSERT_CARGS, NULL, ASSERT_STYLE_EQP,            #A, #B,            (A),            (B));}}while(0)
#define ASSERT_NEQI(A,B)        do{if((A)==(B))   {assert_format(ASSERT_CARGS, NULL, ASSERT_STYLE_NEQI,           #A, #B,  (intmax_t)(A),  (intmax_t)(B));}}while(0)
#define ASSERT_NEQU(A,B)        do{if((A)==(B))   {assert_format(ASSERT_CARGS, NULL, ASSERT_STYLE_NEQU,           #A, #B, (uintmax_t)(A), (uintmax_t)(B));}}while(0)
#define ASSERT_NEQP(A,B)        do{if((A)==(B))   {assert_format(ASSERT_CARGS, NULL, ASSERT_STYLE_NEQP,           #A, #B,            (A),            (B));}}while(0)
#define ASSERT_FALSE(A)         do{if((A))        {assert_format(ASSERT_CARGS, NULL, ASSERT_STYLE_ISFALSE,        #A, (A));}}while(0)
#define ASSERT_TRUE(A)          do{if((A)!=0)     {assert_format(ASSERT_CARGS, NULL, ASSERT_STYLE_ISTRUE,         #A, (A));}}while(0)
#define ASSERT_POW2(A)          do{if((A)&((A)-1)){assert_format(ASSERT_CARGS, NULL, ASSERT_STYLE_ISPOW2,         #A, (A));}}while(0)
#define ASSERT_ALIGN(x,a)       do{if(((uintptr_t)(void const*)(x) % (a)) != 0){assert_format(ASSERT_CARGS, NULL, ASSERT_STYLE_ISALIGN, #a, (a), #x, (x), (uintptr_t)(void const*)(x) % (a));}}while(0)




__attribute__ ((unused))
__attribute__ ((format (printf, 6, 0)))
static void assert_format_va
(int id, char const * file, int line, char const * fn, char const * exp, char const * fmt, va_list va)
{
	fprintf (stderr, ASSERT_STYLE_ASSERT " " ASSERT_STYLE_FILENAME ":" ASSERT_STYLE_LINE " " ASSERT_STYLE_FNAME "() ", id, file, line, fn);
	if (errno != 0) {fprintf (stderr, " " ASSERT_STYLE_ERRNO " ", strerror (errno), errno);}
	if (exp){fprintf (stderr, ": "ASSERT_STYLE_EXP " ", exp);}
	if (fmt)
	{
		vfprintf (stderr, fmt, va);
	}
	fprintf (stderr, "\n");
	fflush (stderr);
}


__attribute__ ((noreturn))
__attribute__ ((format (printf, 6, 0)))
static void assert_format 
(int id,char const * file,int line,char const * fn,char const * exp,char const * fmt,...)
{
/*
#ifdef _WIN32
	file = strrchr(file, '\\') + 1;
#else
	file = strrchr(file, '/') + 1;
#endif
*/
	va_list va;
	va_start (va, fmt);
	assert_format_va (id, file, line, fn, exp, fmt, va);
	va_end (va);
	exit (1);
}





