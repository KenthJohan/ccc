/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "csc_gcam.h"
#include "csc_xlog.h"



static void GLAPIENTRY csc_sdlglew_gldebug_callback
(GLenum source, GLenum type, GLuint id,GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	(void)source;
	(void)type;
	(void)id;
	(void)severity;
	(void)length;
	(void)userParam;
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		XLOG (XLOG_ERR, XLOG_OPENGL, "%s [%x]\n", message, severity);
		//abort();
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		XLOG (XLOG_ERR, XLOG_OPENGL, "%s [%x]\n", message, severity);
		//abort();
		break;
	case GL_DEBUG_SEVERITY_LOW:
		XLOG (XLOG_ERR, XLOG_OPENGL, "%s [%x]\n", message, severity);
		//abort();
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		XLOG (XLOG_INF, XLOG_OPENGL, "%s [%x]\n", message, severity);
		break;
	default:
		XLOG (XLOG_INF, XLOG_OPENGL, "%s [%x]\n", message, severity);
		break;
	}
}



/*
 *
*/
void csc_sdlglew_create_window (SDL_Window ** window, SDL_GLContext * context, const char *title, int x, int y, int w, int h, Uint32 flags)
{
	if (SDL_Init (SDL_INIT_VIDEO) != 0)
	{
		XLOG (XLOG_ERR, XLOG_SDL, "There was an error initializing the SDL library: %s", SDL_GetError());
		exit (1);
	}

	//srand(int) becomes unpredictable after this
	(*window) = SDL_CreateWindow (title, x, y, w, h, flags);
	if ((*window) == NULL)
	{
		XLOG (XLOG_ERR, XLOG_SDL, " Could not create SDL_Window: %s", SDL_GetError());
		exit (1);
	}
	srand(42);

	(*context) = SDL_GL_CreateContext (*window);
	if ((*context) == NULL)
	{
		XLOG (XLOG_ERR, XLOG_SDL, " Could not create SDL_GLContext: %s", SDL_GetError());
		exit (1);
	}

	{
		glewExperimental = 1;
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			XLOG (XLOG_ERR, XLOG_SDL, " %s", glewGetErrorString (err));
			exit (1);
		}
		XLOG (XLOG_INF, XLOG_SDL, " Using GLEW %s", glewGetString (GLEW_VERSION));
	}

	// Enable the debug callback
	glEnable (GL_DEBUG_OUTPUT);
	glEnable (GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback (csc_sdlglew_gldebug_callback, NULL);
	//glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
	glDebugMessageControl (GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_FALSE);//Disable all messages
	glDebugMessageControl (GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DONT_CARE, 0, NULL, GL_TRUE);//Enable error messages
	//glDebugMessageControl (GL_DEBUG_SOURCE_API, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_FALSE);

	glEnable (GL_DEPTH_TEST);
	//glEnable (GL_PROGRAM_POINT_SIZE_EXT);
	//glEnable (GL_VERTEX_PROGRAM_POINT_SIZE);
	//glEnable (GL_TEXTURE_2D);
	//glPointSize (50.0f);
	//glLineWidth (20.0f);
}




#define CSC_SDLGLEW_RUNNING    UINT32_C (0x00000001)
#define CSC_SDLGLEW_FULLSCREEN UINT32_C (0x00000002)
#define CSC_SDLGLEW_FULLSCREEN UINT32_C (0x00000002)

static void csc_sdlglew_resize (SDL_Window * window, struct csc_gcam * gcam)
{
	int w;
	int h;
	SDL_GetWindowSize (window, &w, &h);
	gcam->w = w;
	gcam->h = h;
	glViewport (0, 0, w, h);
	XLOG (XLOG_INF, XLOG_SDL, " SDL_WINDOWEVENT_RESIZED: %i %i\n", w, h);
}


static void csc_sdlglew_event_loop (SDL_Window * window, SDL_Event * event, uint32_t * flags, struct csc_gcam * gcam)
{
	switch (event->type)
	{
	case SDL_QUIT:
		(*flags) &= ~CSC_SDLGLEW_RUNNING;
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_RESIZED)
		{
			csc_sdlglew_resize (window, gcam);
		}
		break;
	case SDL_KEYDOWN:
		switch (event->key.keysym.sym)
		{
		case SDLK_ESCAPE:
			(*flags) &= ~CSC_SDLGLEW_RUNNING;
			break;

		case 'f':
			(*flags) ^= CSC_SDLGLEW_FULLSCREEN;
			if ((*flags) & CSC_SDLGLEW_FULLSCREEN)
			{
				SDL_SetWindowFullscreen (window, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
			else
			{
				SDL_SetWindowFullscreen (window, SDL_WINDOW_OPENGL);
			}
			csc_sdlglew_resize (window, gcam);
			break;

		default:
			break;
		}
		break;
	}
}
