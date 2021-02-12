#pragma once

#include "csc/csc_crossos.h"
#include "csc/csc_basic.h"
#include "csc/csc_debug.h"
#include "csc/csc_math.h"

#include <stdio.h>



#define MYAPI_POINTCLOUD 1
#define MYAPI_IMG_LAYOUT 2
#define MYAPI_IMG_CONTENT 3
#define MYAPI_IMG_POSITION 4


struct myapi_pkg
{
	uint32_t type;
};


struct myapi_pointcloud
{
	struct myapi_pkg pkg;
	uint32_t from;
	uint32_t count;
	v4f32 data[0];
};


struct myapi_img_layout
{
	struct myapi_pkg pkg;
	uint32_t index;
	uint32_t unit;
	uint32_t width;
	uint32_t height;
	uint32_t depth;
};


struct myapi_img_content
{
	struct myapi_pkg pkg;
	uint32_t index;
	uint32_t layer;
	uint32_t data[0];
};


struct myapi_img_position
{
	struct myapi_pkg pkg;
	uint8_t index;
	v4f32 position;
};





















