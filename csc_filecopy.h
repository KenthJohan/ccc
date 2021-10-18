/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdio.h>

static void csc_filecopy_tofolder (char const * filename, char const * destination_folder)
{
	char buf[100];
	snprintf (buf, sizeof (buf), "cp %s %s/%s", filename, destination_folder, filename);
	//printf ("%s\n", buf);
	FILE * f = popen (buf, "r");
	pclose (f);
}
