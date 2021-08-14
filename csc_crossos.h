/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once


// This will ensure that the ANSI I/O standards are preferred over Microsoft's.
//
// qtcreator pro:
// DEFINES += __USE_MINGW_ANSI_STDIO=1
//
#define __USE_MINGW_ANSI_STDIO 1



#if defined(WIN32)
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

/**
 * @brief https://superuser.com/questions/413073/windows-console-with-ansi-colors-handling
 */
static void csc_crossos_enable_ansi_color ()
{
#if defined(WIN32)
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode (hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode (hOut, dwMode);
#endif
}


