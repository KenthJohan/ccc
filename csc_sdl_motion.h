/* SPDX-License-Identifier: GPL-2.0 */
#pragma once
#include <SDL2/SDL.h>

#include "csc_math.h"

void csc_sdl_motion_wasd (const Uint8 * keyboard, v3f32 d)
{
	d [0] = (keyboard [SDL_SCANCODE_A] - keyboard [SDL_SCANCODE_D]);
	d [1] = (keyboard [SDL_SCANCODE_LCTRL] - keyboard [SDL_SCANCODE_SPACE]);
	d [2] = (keyboard [SDL_SCANCODE_W] - keyboard [SDL_SCANCODE_S]);
}


void csc_sdl_motion_pyr (const Uint8 * keyboard, v3f32 pyr)
{
	pyr[0] = (keyboard [SDL_SCANCODE_DOWN] - keyboard [SDL_SCANCODE_UP]);
	pyr[1] = (keyboard [SDL_SCANCODE_RIGHT] - keyboard [SDL_SCANCODE_LEFT]);
	pyr[2] = (keyboard [SDL_SCANCODE_E] - keyboard [SDL_SCANCODE_Q]);
}
