#include "csc_assert.h"
#include "csc_rgb.h"



struct csc_f32rgba table[256];

void rgb676_print()
{
	for(int r = 0; r <= CSC_RGB676_RMAX; ++r)
	{
		for(int g = 0; g <= CSC_RGB676_GMAX; ++g)
		{
			for(int b = 0; b <= CSC_RGB676_BMAX; ++b)
			{
				uint32_t i = CSC_RGB676_INDEX(r,g,b);
				printf ("%03i ", i);
				table[i].r = (float)r / (float)CSC_RGB676_RMAX;
				table[i].g = (float)g / (float)CSC_RGB676_RMAX;
				table[i].b = (float)b / (float)CSC_RGB676_RMAX;
			}
		}
	}
	printf ("\n");

	for(int i = 0; i < 256; ++i)
	{
		printf ("{%10.8f, %10.8f, %10.8f}\n", table[i].r, table[i].g, table[i].b);
	}
}

void rgb884_print()
{
	for(uint32_t r = 0; r <= CSC_RGB884_RMAX; ++r)
	{
		for(uint32_t g = 0; g <= CSC_RGB884_GMAX; ++g)
		{
			for(uint32_t b = 0; b <= CSC_RGB884_BMAX; ++b)
			{
				uint32_t i = CSC_RGB884_INDEX(r,g,b);
				//printf ("%03i %03i %03i : %03i\n", r, g, b, i);
				table[i].r = (float)r / (float)CSC_RGB884_RMAX;
				table[i].g = (float)g / (float)CSC_RGB884_GMAX;
				table[i].b = (float)b / (float)CSC_RGB884_BMAX;
			}
		}
	}
	//printf ("\n");

	for(int i = 0; i < 256; ++i)
	{
		printf ("vec3(%10.8f, %10.8f, %10.8f),\n", table[i].r, table[i].g, table[i].b);
	}
}


int main (int argc, char const * argv [])
{

	rgb884_print();

	return EXIT_SUCCESS;
}
