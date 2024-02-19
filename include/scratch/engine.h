/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/


#ifndef _SCRATCH_ENGINE_H
#define _SCRATCH_ENGINE_H

#include "scratch/_scratch.h"
#include "scratch/project.h"


typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
} scEngine;

scEngine *SC_FASTCALL scEngine_new(scProject project);

void SC_FASTCALL scEngine_free(scEngine *engine);


#endif