#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>



int main (int argc, char * argv [])
{
	uint8_t x[66560];
	memset (x, 0, sizeof (x));
	return x[0];
}
