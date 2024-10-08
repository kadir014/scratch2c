/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/


#ifndef _SCRATCH_MOTION_H
#define _SCRATCH_MOTION_H

#include "scratch/_scratch.h"
#include "scratch/engine/sprite.h"


/**
 * @brief Move sprite towards its direction.
 * 
 * @param sprite Sprite
 * @param steps Pixels to move
 */
static inline void SC_FASTCALL scSprite_move(scSprite *sprite, sc_real steps) {
    // TODO: No need for trig every function call if we store direction vector instead of angle internally.
    sprite->x += sc_cos(sprite->angle * SC_DEG_TO_RAD) * steps;
    sprite->y += sc_sin(sprite->angle * SC_DEG_TO_RAD) * steps;
}

/**
 * @brief Change sprite's locattion
 * 
 * @param sprite Sprite
 * @param x New X position
 * @param y New Y position
 */
static inline void SC_FASTCALL scSprite_goto(scSprite *sprite, sc_real x, sc_real y) {
    sprite->x = x;
    sprite->y = y;
}


#endif