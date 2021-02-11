#pragma once

#include "../csc_crossos.h"
#include "../csc_basic.h"
#include "../csc_debug.h"
#include "../csc_gl.h"

#include "csc_glimage.h"
#include "csc_glpointcloud.h"
#include "csc_gltex.h"
#include "netgl.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stdio.h>




void netgl_recv
(
	struct myapi_pkg * pkg,
	struct csc_glpointcloud * g_pointcloud,
	struct csc_glimgcontext * g_img
)
{
	struct myapi_pointcloud * pkg_pointcloud;
	struct myapi_img * pkg_img;
	switch (pkg->type)
	{
	case MYAPI_POINTCLOUD:
		pkg_pointcloud = (void*)pkg;
		csc_glpointcloud_update (g_pointcloud, pkg_pointcloud->from, pkg_pointcloud->count, pkg_pointcloud->data);
		break;
	case MYAPI_IMG:
		pkg_img = (void*)pkg;
		break;
	}
}













