/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/


#ifndef _SCRATCH_ENGINE_H
#define _SCRATCH_ENGINE_H

#include "scratch/_scratch.h"
#include "scratch/engine/project.h"
#include "scratch/engine/renderer.h"
#include "scratch/core/prng.h"


typedef struct {
    SDL_Window *window;
    scRenderer *renderer;
    bool is_running;

    scPRNGAlg prng_alg;
    scPRNGState prng_state;

    double clock_frequency;
    double clock_start;
    double clock_last;
    double clock_timer;
    double fps;

    int mouse_x;
    int mouse_y;
    bool mouse_pressed;
} scEngine;

scEngine *SC_FASTCALL scEngine_new(scProject project);

void SC_FASTCALL scEngine_free(scEngine *engine);

void SC_FASTCALL scEngine_tick(scEngine *engine);

void SC_FASTCALL scEngine_render(scEngine *engine, scProject *project);

void SC_FASTCALL scEngine_clear(scEngine *engine);

void SC_FASTCALL scEngine_flush(scEngine *engine);

/**
 * @brief Return random sc_real in range [lower, higher).
 * 
 * @param engine Engine
 * @param lower Lower limit
 * @param higher Higher limit
 * @return sc_real
 */
sc_real SC_FASTCALL scEngine_random_real(
    scEngine *engine,
    sc_real lower,
    sc_real higher
);


#endif