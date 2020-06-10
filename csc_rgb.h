/*
MIT License

Copyright (c) 2019 CSC Project

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once
#include <stddef.h> //offsetof
#include <stdint.h> //offsetof



#define CSC_RGB676_INDEX(R,G,B) ((42*(R))+(6*G)+(B))
#define CSC_RGB676_RMAX 5
#define CSC_RGB676_GMAX 6
#define CSC_RGB676_BMAX 5


#define CSC_RGB884_INDEX(R,G,B) (((R) << 5) | ((G) << 2) | (B))
#define CSC_RGB884_RMAX 7
#define CSC_RGB884_GMAX 7
#define CSC_RGB884_BMAX 3

struct csc_f32rgba
{
	float r;
	float g;
	float b;
	float a;
};

