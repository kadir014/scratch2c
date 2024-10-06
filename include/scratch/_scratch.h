/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/


#ifndef _SCRATCH_INTERNAL_H
#define _SCRATCH_INTERNAL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <glad/glad.h>
#include <GL/gl.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "scratch/core/platform.h"
#include "scratch/core/types.h"


#if defined(SC_PLATFORM_WEB)
    #include <emscripten.h>
#elif defined(SC_PLATFORM_WINDOWS)
    #include <windows.h>
#endif

#ifdef SC_PLATFORM_WEB
    #define SC_EXIT(exit_code) (emscripten_force_exit(exit_code))
#else
    #define SC_EXIT(exit_code) (exit(exit_code))
#endif


#endif