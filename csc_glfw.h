/* SPDX-License-Identifier: GPL-2.0 */
#pragma once

#include <GLFW/glfw3.h>

uint32_t csc_glfw_copy_required_instance_extensions (char const * ext [], uint32_t count)
{
	uint32_t gcount = 0;
	const char** gext = glfwGetRequiredInstanceExtensions (&gcount);
	uint32_t n;
	for (n = 0; (n < gcount) && (n < count); ++n)
	{
		ext [n] = gext [n];
	}
	return n;
}
