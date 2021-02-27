/* SPDX-License-Identifier: GPL-2.0 */
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <inttypes.h>

#include "csc_basic.h"


static char const * csc_type_tostr (enum csc_type t)
{
	switch (t)
	{
	case CSC_TYPE_STRING:return "string";
	case CSC_TYPE_INT:return "int";
	case CSC_TYPE_LONG:return "long";
	case CSC_TYPE_U8:return "u8";
	case CSC_TYPE_U16:return "u16";
	case CSC_TYPE_U32:return "u32";
	case CSC_TYPE_U64:return "u64";
	case CSC_TYPE_I8:return "i8";
	case CSC_TYPE_I16:return "i16";
	case CSC_TYPE_I32:return "i32";
	case CSC_TYPE_I64:return "i64";
	case CSC_TYPE_FLOAT:return "float";
	case CSC_TYPE_DOUBLE:return "double";
	default:return "unknown";
	}
}


static void csc_type_print (enum csc_type type, void * value, FILE * f)
{
	switch (type)
	{
	case CSC_TYPE_DOUBLE:
		fprintf (f, "%f\n", *(double*)value);
		break;
	case CSC_TYPE_FLOAT:
		fprintf (f, "%f\n", *(float*)value);
		break;
	case CSC_TYPE_INT:
		fprintf (f, "%i\n", *(int*)value);
		break;
	case CSC_TYPE_LONG:
		fprintf (f, "%li\n", *(long*)value);
		break;
	case CSC_TYPE_I8:
		fprintf (f, "%i\n", *(int8_t*)value);
		break;
	case CSC_TYPE_I16:
		fprintf (f, "%i\n", *(int16_t*)value);
		break;
	case CSC_TYPE_I32:
		fprintf (f, "%i\n", *(int32_t*)value);
		break;
	case CSC_TYPE_I64:
		fprintf (f, "%ji\n", *(int64_t*)value);
		break;
	case CSC_TYPE_U8:
		fprintf (f, "%u\n", *(uint8_t*)value);
		break;
	case CSC_TYPE_U16:
		fprintf (f, "%u\n", *(uint16_t*)value);
		break;
	case CSC_TYPE_U32:
		fprintf (f, "%u\n", *(uint32_t*)value);
		break;
	case CSC_TYPE_U64:
		fprintf (f, "%ju\n", *(uint64_t*)value);
		break;
	case CSC_TYPE_STRING:
		fprintf (f, "%s\n", *(char**)value);
		break;
	default:
		break;
	}
}


static void csc_type_printflag (enum csc_type type, void * value, uint64_t flag)
{
	switch (type)
	{
	case CSC_TYPE_INT:
		fprintf (stdout, "%s\n", (*(int*)value & (int)flag) ? "True" : "False");
		break;
	case CSC_TYPE_LONG:
		fprintf (stdout, "%s\n", (*(long*)value & (int)flag) ? "True" : "False");
		break;
	case CSC_TYPE_I8:
		fprintf (stdout, "%s\n", (*(int8_t*)value & (int8_t)flag) ? "True" : "False");
		break;
	case CSC_TYPE_I16:
		fprintf (stdout, "%s\n", (*(int16_t*)value & (int16_t)flag) ? "True" : "False");
		break;
	case CSC_TYPE_I32:
		fprintf (stdout, "%s\n", (*(int32_t*)value & (int32_t)flag) ? "True" : "False");
		break;
	case CSC_TYPE_I64:
		fprintf (stdout, "%s\n", (*(int64_t*)value & (int64_t)flag) ? "True" : "False");
		break;
	case CSC_TYPE_U8:
		fprintf (stdout, "%s\n", (*(uint8_t*)value & (uint8_t)flag) ? "True" : "False");
		break;
	case CSC_TYPE_U16:
		fprintf (stdout, "%s\n", (*(uint16_t*)value & (uint16_t)flag) ? "True" : "False");
		break;
	case CSC_TYPE_U32:
		fprintf (stdout, "%s\n", (*(uint32_t*)value & (uint32_t)flag) ? "True" : "False");
		break;
	case CSC_TYPE_U64:
		fprintf (stdout, "%s\n", (*(uint64_t*)value & (uint64_t)flag) ? "True" : "False");
		break;
	default:
		break;
	}
}
