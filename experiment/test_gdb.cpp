#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <iostream>



int main (int argc, char * argv [])
{
	uint8_t x[66560];
	memset (x, 0, sizeof (x));
	std::cout << (intptr_t)x << std::endl;
	return x[0];
}
