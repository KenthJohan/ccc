#pragma once

#include <stdio.h>
#include <stdint.h>
#include "csc_debug.h"
#include "csc_linmat.h"





int main(int argc, char * argv[])
{
	ASSERT (argc);
	ASSERT (argv);

	{
		float plane[4];
		float a[4] = {1.0f, 2.0, -2.0f, 0.0f};
		float b[4] = {3.0f, -2.0, 1.0f, 0.0f};
		float c[4] = {5.0f, 1.0, -4.0f, 0.0f};
		csc_linmat_plane_from_3points (plane, a, b, c);
		ASSERT (plane[0] == 11.0f);
		ASSERT (plane[1] == 16.0f);
		ASSERT (plane[2] == 14.0f);
		ASSERT (plane[3] == -15.0f);
	}

	{
		float plane[4] = {2.0f, 4.0f, 3.0f, 5.0f};
		float x[3] = {1.0f, 2.0f, 3.0f};
		float l = csc_linmat_plane_point_distance (plane, x);
		ASSERT (fabs (l - 4.4566881162492) < 0.01f);
	}



	return 0;
}
