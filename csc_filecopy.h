/* SPDX-License-Identifier: GPL-2.0 */
#pragma once

#include <stdio.h>

void csc_filecopy_tofolder (char const * filename, char const * destination_folder)
{
	char buf[100];
	snprintf (buf, sizeof (buf), "cp %s %s/%s", filename, destination_folder, filename);
	//printf ("%s\n", buf);
	FILE * f = popen (buf, "r");
	pclose (f);
}
