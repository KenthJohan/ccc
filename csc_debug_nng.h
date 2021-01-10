/* SPDX-License-Identifier: GPL-2.0 */
/*
https://github.com/nanomsg/nng
*/

#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#include <nng/nng.h>
#include <nng/supplemental/util/platform.h>

#define NNG_EXIT_ON_ERROR(r) nng_exit_on_error(r,__FILE__,__LINE__)
void nng_exit_on_error (int r, char const * file, int line)
{
	if (r != 0)
	{
		fprintf (stderr, "%s:%i: ", file, line);
		perror (nng_strerror (r));
		exit (EXIT_FAILURE);
	}
}
