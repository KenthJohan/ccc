#pragma once

#include <stdio.h>
#include <stdarg.h>
#include "csc_tcol.h"



enum xloglvl
{
	XLOG_ERR,
	XLOG_WRN,
	XLOG_INF
};


enum xlogcategory
{
	XLOG_GENERAL,
	XLOG_SDL,
	XLOG_OPENGL,
	XLOG_FT,//Freetype
	XLOG_ECS,
	XLOG_ECS_ONSET,
	XLOG_ECS_ONADD,
	XLOG_ECS_ONUPDATE,
};

static char const * xloglvl_tostr(enum xloglvl level)
{
	switch (level)
	{
	case XLOG_ERR:
		return TFG(255,50,50) "ERR" TCOL_RST;
		break;
	case XLOG_WRN:
		return TFG(200,200,50) "WRN" TCOL_RST;
		break;
	case XLOG_INF:
		return TFG(180,180,255) "INF" TCOL_RST;
		break;
	}
	return "";
}

static char const * xlogcategory_tostr(enum xlogcategory category)
{
	switch (category)
	{
	case XLOG_GENERAL:
		return TFG(152,90,133) "" TCOL_RST;
		break;
	case XLOG_OPENGL:
		return TFG(152,90,133) "OPENGL   " TCOL_RST;
		break;
	case XLOG_FT:
		return TFG(152,90,133) "FT       " TCOL_RST;
		break;
	case XLOG_ECS:
		return TFG(152,90,133) "ECS      " TCOL_RST;
		break;
	case XLOG_SDL:
		return TFG(152,90,133) "SDL      " TCOL_RST;
		break;
	case XLOG_ECS_ONSET:
		return TFG(152,90,133) "ECS_ONSET" TCOL_RST;
		break;
	case XLOG_ECS_ONADD:
		return TFG(152,90,133) "ECS_ONADD" TCOL_RST;
		break;
	case XLOG_ECS_ONUPDATE:
		return TFG(152,90,133) "ECS_ONUP " TCOL_RST;
		break;
	}
	return "";
}

#define XLOG(level, category, format, ...) xlog(__COUNTER__, __FILE__, __LINE__, __func__, level, category, (format), ## __VA_ARGS__)

void xlog(int counter, char const * file, int line, char const * func, enum xloglvl level, enum xlogcategory category, char const * format, ...)
{
	va_list args;
	va_start (args, format);
	printf (TFG(100,100,100) "@%04i " TCOL_RST "%s %s " TFG(100,100,100) "%s:%i " TFG(130, 110, 60) "%s() " TCOL_RST, counter, xloglvl_tostr(level), xlogcategory_tostr(category), file, line, func);
	vprintf (format, args);
	putc ('\n', stdout);
	va_end (args);
}

