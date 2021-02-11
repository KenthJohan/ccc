#pragma once

#include "csc/csc_crossos.h"
#include "csc/csc_basic.h"
#include "csc/csc_debug.h"

#include <stdio.h>



#define MYAPI_POINTCLOUD 1
#define MYAPI_IMG 2


struct myapi_pkg
{
	uint32_t type;
};


struct myapi_pointcloud
{
	struct myapi_pkg pkg;
	uint32_t from;
	uint32_t count;
	struct v4f32 data[0];
};


struct myapi_img
{
	struct myapi_pkg pkg;
	uint32_t from;
	uint32_t count;
	struct v4f32 data[0];
};




















