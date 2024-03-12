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
unsigned int g_shader;
unsigned int g_vao;

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

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
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

    engine->renderer = scRenderer_new(engine->window);
    if (!engine->renderer) {
        SDL_DestroyWindow(engine->window);
        _quit();
        exit(EXIT_FAILURE);
    }
    
    char title[64];
    sprintf(
        title,
        "Scratch2C - OpenGL %d.%d %s - %s",
        engine->renderer->gl_version.major,
        engine->renderer->gl_version.minor,
        scRendererGLProfile_as_str(engine->renderer->gl_profile),
        engine->renderer->gpu_info.name
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

    engine->is_running = false;

    engine->clock_frequency = (double)SDL_GetPerformanceFrequency();
    engine->clock_start = (double)SDL_GetPerformanceCounter() / engine->clock_frequency;
    engine->clock_last = 0.0;

    engine->mouse_x = 0;
    engine->mouse_y = 0;
    engine->mouse_pressed = false;

    // float vertices[] = {
    //     -0.5f, -0.5f, 0.0f,
    //     0.5f, -0.5f, 0.0f,
    //     0.0f,  0.5f, 0.0f
    // };
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // Top Right
         0.5f, -0.5f, 0.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f,  // Bottom Left
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,   // Top Left
         0.5f,  0.5f, 0.0f
    };

    const char *vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        fprintf(stderr, "shader compilation error\n");
        exit(EXIT_FAILURE);
    }

    const char *fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.0f, 1.0f, 0.2f, 1.0f);\n"
"}\0";

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        fprintf(stderr, "shader compilation error\n");
        exit(EXIT_FAILURE);
    }

    g_shader = glCreateProgram();
    glAttachShader(g_shader, vertexShader);
    glAttachShader(g_shader, fragmentShader);
    glLinkProgram(g_shader);
    glGetProgramiv(g_shader, GL_LINK_STATUS, &success);
    if(!success) {
        fprintf(stderr, "shader linking error\n");
        exit(EXIT_FAILURE);
    }

    glGenVertexArrays(1, &g_vao);
    glBindVertexArray(g_vao);
    // 2. copy our vertices array in a buffer for OpenGL to use
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

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
    glUseProgram(g_shader);
    glBindVertexArray(g_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

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