/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/

#include "scratch/engine/engine.h"
#include "scratch/scratch.h"


/**
 * @brief Release all SDL modules.
 */
static inline void _quit() {
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

    engine->prng_alg = scPRNGAlg_DEFAULT;
    engine->prng_state = scPRNG_init(engine->prng_alg);
    srand(time(NULL));

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

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    engine->window = SDL_CreateWindow(
        "Scratch2C",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        project.stage_width,
        project.stage_height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
    );
    if (!engine->window) {
        fprintf(stderr, SDL_GetError());
        _quit();
        exit(EXIT_FAILURE);
    }

    engine->renderer = scRenderer_new(engine->window, project.stage_width, project.stage_height);
    if (!engine->renderer) {
        SDL_DestroyWindow(engine->window);
        _quit();
        exit(EXIT_FAILURE);
    }
    
    char title[64];
    sprintf(
        title,
        "Scratch2C Project - OpenGL %d.%d %s",
        engine->renderer->gl_version.major,
        engine->renderer->gl_version.minor,
        scRendererGLProfile_as_str(engine->renderer->gl_profile)
    );
    SDL_SetWindowTitle(engine->window, title);
    SDL_SetWindowTitle(engine->window, title);

    SDL_Surface *window_icon = IMG_Load("assets/scratch_icon.png");
    if (window_icon) {
        SDL_SetWindowIcon(engine->window, window_icon);
        SDL_FreeSurface(window_icon);
    }
    else {
        fprintf(stderr, IMG_GetError());
    }

    int gl_major_version, gl_minor_version;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &gl_major_version);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &gl_minor_version);

    printf(
        "Scratch engine initialized\n"
        "--------------------------\n"
        "Version: %s\n"
        "SDL: %d.%d.%d\n"
        "SDL_IMG: %d.%d.%d\n"
        "SDL_TTF: %d.%d.%d\n"
        "OpenGL: %d.%d\n",
        SC_VERSION_STRING,
        SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL,
        SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_PATCHLEVEL,
        SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION, SDL_TTF_PATCHLEVEL,
        gl_major_version, gl_minor_version
    );

    engine->is_running = false;

    engine->clock_frequency = (double)SDL_GetPerformanceFrequency();
    engine->clock_start = (double)SDL_GetPerformanceCounter() / engine->clock_frequency;
    engine->clock_last = 0.0;
    engine->clock_timer = 0.0;

    engine->mouse_x = 0;
    engine->mouse_y = 0;
    engine->mouse_pressed = false;

    return engine;
}

void SC_FASTCALL scEngine_free(scEngine *engine) {
    if (!engine) return;

    SDL_DestroyWindow(engine->window);
    scRenderer_free(engine->renderer);
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
    engine->mouse_pressed = (mouse_state & SDL_BUTTON(1)) | 
                            (mouse_state & SDL_BUTTON(2)) | 
                            (mouse_state & SDL_BUTTON(3));

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            engine->is_running = false;
        }
    }
}

void SC_FASTCALL scEngine_render(scEngine *engine, scProject *project) {
    //glUseProgram(g_shader);
    //glBindVertexArray(g_vao);
    //glDrawArrays(GL_TRIANGLES, 0, 6);

    // for (size_t i = 0; i < project->targets_size; i++) {
    //     scSprite target = project->targets[i];
    //     if (!target.visible) continue;

    //     SDL_Texture *texture = target.costumes[0].texture;

    //     sc_real x = target.x;
    //     sc_real y = target.y;
    //     sc_scratch_to_screen_space(project, &x, &y);

    //     int texture_width, texture_height;
    //     SDL_QueryTexture(texture, NULL, NULL, &texture_width, &texture_height);

    //     // Scale by sprite size
    //     // float width = texture_width * transform->scale.x;
    //     // float height = texture_height * transform->scale.y;
    //     float width = texture_width;
    //     float height = texture_height;

    //     SDL_FRect dest_rect = {
    //         x - width / 2.0,
    //         y - height / 2.0,
    //         width,
    //         height
    //     };

    //     SDL_RenderCopyExF(
    //         engine->renderer,
    //         texture,
    //         NULL,
    //         &dest_rect,
    //         target.angle,
    //         NULL,
    //         SDL_FLIP_NONE
    //     );
    // }
    engine->renderer->sprite_vao_n = 0;
    engine->renderer->sprite_vertices_n = 0;
    for (size_t i = 0; i < project->targets_size; i++) {
        scSprite *sprite =&project->targets[i];
        if (sprite->is_stage) continue;
        scRenderer_add_sprite(engine->renderer, sprite);
    }

    glBindBuffer(GL_ARRAY_BUFFER, engine->renderer->sprite_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, (engine->renderer->sprite_vertices_n) * sizeof(float), engine->renderer->sprite_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    scRenderer_render(engine->renderer);
}

void SC_FASTCALL scEngine_clear(scEngine *engine) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void SC_FASTCALL scEngine_flush(scEngine *engine) {
    SDL_GL_SwapWindow(engine->window);
}

sc_real SC_FASTCALL scEngine_random_real(
    scEngine *engine,
    sc_real lower,
    sc_real higher
) {
    sc_uint64 randn;
    switch (engine->prng_alg) {
        default:
        case scPRNGAlg_DEFAULT:
            randn = rand();
            break;

        case scPRNGAlg_XORSHIFT128P:
            randn = sc_xorshift128p(&engine->prng_state);
            break;
    }

    sc_real normal = randn / (sc_real)RAND_MAX;
    return lower + normal * (higher - lower);
}