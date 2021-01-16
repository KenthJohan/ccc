/* SPDX-License-Identifier: GPL-2.0 */
#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "csc_basic.h"


void csc_printinfo()
{
#ifdef BUILD_DATE
	printf ("BUILD_DATE: " STRINGIFY(BUILD_DATE) "\n");
#endif

#ifdef GIT_VERSION
	printf ("GIT_VERSION: " STRINGIFY(GIT_VERSION) "\n");
#endif

#ifdef _WIN32
	printf ("Built on Windows 32\n");
#endif

#ifdef _WIN64
	printf ("Built on Windows 64\n");
#endif

#ifdef __linux__
	printf ("Built on Linux\n");
#endif
}
