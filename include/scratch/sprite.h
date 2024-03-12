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
    SDL_Surface *surface;
    const char *filename;
} scCostume;

static inline scCostume SC_FASTCALL scCostume_load(
    const char *filepath
) {
    // SDL_Texture *texture = IMG_LoadTexture(renderer, filepath);
    // if (!texture) {
    //     fprintf(stderr, IMG_GetError());
    //     exit(EXIT_FAILURE);
    // }

    SDL_Surface *surf = IMG_Load(filepath);

    return (scCostume){
        .filename=filepath,
        .surface=surf
    };
}


typedef struct {
    bool is_stage;
    scCostume costumes[10];
    size_t max_costumes;
    size_t current_costume;
    sc_real x;
    sc_real y;
    sc_real angle;
    bool visible;
    bool draggable;
} scSprite;


#endif