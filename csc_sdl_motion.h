/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once
#include <SDL2/SDL.h>

#include "csc_math.h"
#include "csc_v3f32.h"

void csc_sdl_motion_wasd (const Uint8 * keyboard, v3f32 * d)
{
	d->x = (keyboard [SDL_SCANCODE_A] - keyboard [SDL_SCANCODE_D]);
	d->y = (keyboard [SDL_SCANCODE_LCTRL] - keyboard [SDL_SCANCODE_SPACE]);
	d->z = (keyboard [SDL_SCANCODE_W] - keyboard [SDL_SCANCODE_S]);
}


void csc_sdl_motion_pyr (const Uint8 * keyboard, v3f32 * pyr)
{
	pyr->x = (keyboard [SDL_SCANCODE_DOWN] - keyboard [SDL_SCANCODE_UP]);
	pyr->y = (keyboard [SDL_SCANCODE_RIGHT] - keyboard [SDL_SCANCODE_LEFT]);
	pyr->z = (keyboard [SDL_SCANCODE_E] - keyboard [SDL_SCANCODE_Q]);
}
