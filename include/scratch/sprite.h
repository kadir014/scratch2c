/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/


#ifndef _SCRATCH_SPRITE_H
#define _SCRATCH_SPRITE_H

#include "scratch/_scratch.h"


typedef struct {
    SDL_Texture *texture;
    const char *filename;

} scCostume;


typedef struct {
    scCostume costumes[10];
    size_t max_costumes;
    size_t current_costume;
    double x;
    double y;
    double angle;
    bool visible;
    bool draggable;
} scSprite;


#endif