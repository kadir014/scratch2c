/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/

#include "scratch/engine.h"


/**
 * @brief Release all SDL modules.
 */
static inline void _quit() {
    SDL_Quit();
    TTF_Quit();
    IMG_Quit();
}


/**
 * @brief Convert Scratch coordinates to screen space.
 * 
 * @param x X
 * @param y Y
 */
static inline void SC_FASTCALL sc_scratch_to_screen_space(scProject *project, sc_real *x, sc_real *y) {
    *x += project->stage_width2;
    *y = -(*y) + project->stage_height2;
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

    // Initialize PRNG
    srand(time(NULL));

    engine->is_running = false;

    engine->clock_frequency = (double)SDL_GetPerformanceFrequency();
    engine->clock_start = (double)SDL_GetPerformanceCounter() / engine->clock_frequency;
    engine->clock_last = 0.0;

    engine->mouse_x = 0;
    engine->mouse_y = 0;
    engine->mouse_pressed = false;

    return engine;
}

void SC_FASTCALL scEngine_free(scEngine *engine) {
    if (!engine) return;

    SDL_DestroyRenderer(engine->renderer);
    SDL_DestroyWindow(engine->window);
    free(engine);
    _quit();
}

void SC_FASTCALL scEngine_tick(scEngine *engine) {
    engine->clock_timer = (double)SDL_GetPerformanceCounter() / engine->clock_frequency - engine->clock_start;

    engine->fps = 1.0 / (engine->clock_timer - engine->clock_last);
    engine->clock_last = engine->clock_timer;

    // char title[64];
    // sprintf(title, "Scratch2C Window - SDL2 Direct3D Renderer - %.1fFPS", fps);
    // SDL_SetWindowTitle(engine->window, title);

    sc_uint32 mouse_state = SDL_GetMouseState(&engine->mouse_x, &engine->mouse_y);
    engine->mouse_pressed = mouse_state & SDL_BUTTON(1) | 
                            mouse_state & SDL_BUTTON(2) | 
                            mouse_state & SDL_BUTTON(3);

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            engine->is_running = false;
        }
    }
}

void SC_FASTCALL scEngine_render(scEngine *engine, scProject *project) {
    for (size_t i = 0; i < project->targets_size; i++) {
        scSprite target = project->targets[i];
        if (!target.visible) continue;

        SDL_Texture *texture = target.costumes[0].texture;

        sc_real x = target.x;
        sc_real y = target.y;
        sc_scratch_to_screen_space(project, &x, &y);

        int texture_width, texture_height;
        SDL_QueryTexture(texture, NULL, NULL, &texture_width, &texture_height);

        // Scale by sprite size
        // float width = texture_width * transform->scale.x;
        // float height = texture_height * transform->scale.y;
        float width = texture_width;
        float height = texture_height;

        SDL_FRect dest_rect = {
            x - width / 2.0,
            y - height / 2.0,
            width,
            height
        };

        SDL_RenderCopyExF(
            engine->renderer,
            texture,
            NULL,
            &dest_rect,
            target.angle,
            NULL,
            SDL_FLIP_NONE
        );
    }
}

void SC_FASTCALL scEngine_clear(scEngine *engine) {
    SDL_SetRenderDrawColor(engine->renderer, 255, 255, 255, 255);
    SDL_RenderClear(engine->renderer);
}

void SC_FASTCALL scEngine_flush(scEngine *engine) {
    SDL_RenderPresent(engine->renderer);
}