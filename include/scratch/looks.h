/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/


#ifndef _SCRATCH_LOOKS_H
#define _SCRATCH_LOOKS_H

#include "scratch/_scratch.h"
#include "scratch/sprite.h"


/**
 * @brief Show sprite.
 * 
 * Category: Looks
 * 
 * @param sprite Sprite
 */
static inline void SC_FASTCALL scSprite_show(scSprite *sprite) {
    sprite->visible = true;
}

/**
 * @brief Hide sprite.
 * 
 * Category: Looks
 * 
 * @param sprite 
 */
static inline void SC_FASTCALL scSprite_hide(scSprite *sprite) {
    sprite->visible = false;
}


#endif