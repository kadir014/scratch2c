/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/

#include "scratch/engine.h"


static void _quit() {
    SDL_Quit();
    TTF_Quit();
    IMG_Quit();
}

scEngine *SC_FASTCALL scEngine_new(scProject project) {
    scEngine *engine = (scEngine *)malloc(sizeof(scEngine));
    if (!engine) {
        fprintf(stderr, "Couldn't allocate memory.\n");
        exit(EXIT_FAILURE);
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
	    fprintf(stderr, "SDL initialization error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
	}

    if (TTF_Init() != 0) {
	    fprintf(stderr, "SDL_ttf initialization error: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
	}

    sc_uint32 img_init_flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(img_init_flags) & img_init_flags)) {
	    fprintf(stderr, "SDL_image initialization error: %s\n", IMG_GetError());
        exit(EXIT_FAILURE);
	}

    engine->window = SDL_CreateWindow(
        "Scratch2C Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        project.stage_width,
        project.stage_height,
        SDL_WINDOW_SHOWN
    );
    if (!engine->window) {
        fprintf(stderr, SDL_GetError());
        _quit();
        exit(EXIT_FAILURE);
    }

    engine->renderer = SDL_CreateRenderer(
        engine->window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!engine->renderer) {
        SDL_DestroyWindow(engine->window);
        _quit();
        fprintf(stderr, SDL_GetError());
        exit(EXIT_FAILURE);
    };

    SDL_SetRenderDrawBlendMode(engine->renderer, SDL_BLENDMODE_BLEND);

    SDL_Surface *window_icon = IMG_Load("assets/scratch_icon.png");
    if (window_icon) {
        SDL_SetWindowIcon(engine->window, window_icon);
        SDL_FreeSurface(window_icon);
    }
    else {
        fprintf(stderr, IMG_GetError());
    }

    return engine;
}

void SC_FASTCALL scEngine_free(scEngine *engine) {
    if (!engine) return;

    SDL_DestroyRenderer(engine->renderer);
    SDL_DestroyWindow(engine->window);
    free(engine);
    _quit();
}