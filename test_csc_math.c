#pragma once

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <float.h>
#include "csc_math.h"





#define ARN 4
#define ACN 2

#define BRN 2
#define BCN 3

int main(int argc, char * argv[])
{
	UNUSED (argc);
	UNUSED (argv);
	float a[ARN*ACN] =
	{
	1.0f, 2.0f, 3.0f, 4.0f,
	5.0f, 6.0f, 7.0f, 8.0f,
	};
	float b[BRN*BCN] =
	{
	1.0f, 2.0f,
	3.0f, 4.0f,
	5.0f, 6.0f
	};
	float c[ARN*BCN];

	memset(c, 0, sizeof(c));
	mvf32_macc (c, a, b, ARN, ACN);
	assert (c[0] == a[0]*b[0] + a[4]*b[1]);
	assert (c[1] == a[1]*b[0] + a[5]*b[1]);
	assert (c[2] == a[2]*b[0] + a[6]*b[1]);
	assert (c[3] == a[3]*b[0] + a[7]*b[1]);


	memset(c, 0, sizeof(c));
	mmf32_macc (c, a, b, ARN, ACN, BCN);
	assert (c[0] == a[0]*b[0] + a[4]*b[1]);
	assert (c[1] == a[1]*b[0] + a[5]*b[1]);
	assert (c[2] == a[2]*b[0] + a[6]*b[1]);
	assert (c[3] == a[3]*b[0] + a[7]*b[1]);
	assert (c[4] == a[0]*b[2] + a[4]*b[3]);
	assert (c[5] == a[1]*b[2] + a[5]*b[3]);
	assert (c[6] == a[2]*b[2] + a[6]*b[3]);
	assert (c[7] == a[3]*b[2] + a[7]*b[3]);
	assert (c[8]  == a[0]*b[4] + a[4]*b[5]);
	assert (c[9]  == a[1]*b[4] + a[5]*b[5]);
	assert (c[10] == a[2]*b[4] + a[6]*b[5]);
	assert (c[11] == a[3]*b[4] + a[7]*b[5]);



	return 0;
}
