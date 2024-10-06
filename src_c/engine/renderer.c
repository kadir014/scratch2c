/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/

#include "scratch/engine/renderer.h"
#include "scratch/core/file.h"


/**
 * @brief Parse the GPU brand name from GL_VENDOR string.
 * 
 * @param vendor_str String returned from glGetString(GL_VENDOR)
 * @return char *
 */
static void parse_brand(char *vendor_str, char *brand_str) {
    char vendor[128];
    for (int i = 0; vendor_str[i]; i++) {
        vendor[i] = tolower(vendor_str[i]);
    }

    if (strstr(vendor, "nvidia") != NULL) {
        strcpy(brand_str, "NVIDIA");
    }
    else if (strstr(vendor, "amd") != NULL) {
        strcpy(brand_str, "AMD");
    }
    else if (strstr(vendor, "intel") != NULL) {
        strcpy(brand_str, "Intel");
    }
    else {
        // Just hope that brand name happens to be in the beginning
        sscanf(vendor, "%s", brand_str);
    }
}

/**
 * @brief Parse the graphics card name from GL_RENDERER string.
 * 
 * @param renderer_str String reutnred from glGetString(GL_RENDERER)
 * @return char *
 */
static void parse_gpu(char *renderer_str, char *gpu_str) {
    char card[64];
    strcpy(card, renderer_str);

    // Remove '/PCIe/SSE2' (and similar)
    char *split_pos = strchr(card, '/');
    if (split_pos != NULL) *split_pos = '\0';

    // Remove 'OpenGL Engine'
    split_pos = strstr(card, "OpenGL Engine");
    if (split_pos != NULL) *split_pos = '\0';

    strcpy(gpu_str, card);
}

static sc_uint32 load_shader(const char *source, sc_uint32 shader_type) {
    sc_uint32 shader_id = glCreateShader(shader_type);
    if (!shader_id) {
        fprintf(stderr, "Shader creation failed.");
        exit(EXIT_FAILURE);
    }
    glShaderSource(shader_id, 1, &source, NULL);
    glCompileShader(shader_id);

    int success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        fprintf(stderr, "Shader compilation failed.");
        exit(EXIT_FAILURE);
    }

    return shader_id;
}

static inline void SC_FASTCALL scratch_to_screen_space(
    sc_real *x,
    sc_real *y,
    sc_real wh,
    sc_real hh
) {
    *x = (*x) + wh;
    *y = -(*y) + hh;
}

static inline void SC_FASTCALL normalize_coords(
    sc_real *x,
    sc_real *y,
    sc_real w,
    sc_real h
) {
    *x = (2.0 * (*x) / w) - 1.0;
    *y = 1.0 - (2.0 * (*y) / h);
}


scRenderer *SC_FASTCALL scRenderer_new(
    SDL_Window *window,
    sc_real viewport_w,
    sc_real viewport_h
) {
    scRenderer *renderer = (scRenderer *)malloc(sizeof(scRenderer));
    if (!renderer) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return NULL;
    }

    renderer->viewport_w = viewport_w;
    renderer->viewport_h = viewport_h;
    renderer->viewport_wh = viewport_w * 0.5;
    renderer->viewport_hh = viewport_h * 0.5;

    renderer->ctx = SDL_GL_CreateContext(window);
    if (!renderer->ctx) {
        fprintf(stderr, SDL_GetError());
        return NULL;
    }
    SDL_GL_MakeCurrent(window, renderer->ctx);

    if (!gladLoaderLoadGL()) {
        fprintf(stderr, "Failed to initialize GLAD.\n");
        return NULL;
    }

    int profile_mask;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &renderer->gl_version.major);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &renderer->gl_version.minor);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &profile_mask);

    switch (profile_mask) {
        case SDL_GL_CONTEXT_PROFILE_CORE:
            renderer->gl_profile = scRendererGLProfile_CORE;
            break;

        case SDL_GL_CONTEXT_PROFILE_COMPATIBILITY:
            renderer->gl_profile = scRendererGLProfile_COMPATIBILITY;
            break;

        default:
        case SDL_GL_CONTEXT_PROFILE_ES:
            renderer->gl_profile = scRendererGLProfile_ES;
            break;
    }

    char *brand = (char *)malloc(sizeof(char) * 16);
    char *gpu = (char *)malloc(sizeof(char) * 64);
    parse_brand((char *)glGetString(GL_VENDOR), brand);
    parse_gpu((char *)glGetString(GL_RENDERER), gpu);
    renderer->gpu_info.name = gpu;
    renderer->gpu_info.brand = brand;

    size_t sprite_vertices_max = sizeof(float) * 500;
    renderer->sprite_vertices = malloc(sprite_vertices_max);
    renderer->sprite_vertices_n = 0;
    renderer->sprite_vao_n = 0;
    glGenBuffers(1, &renderer->sprite_vbo);
    glGenVertexArrays(1, &renderer->sprite_vao);
    glBindVertexArray(renderer->sprite_vao);

    glBindBuffer(GL_ARRAY_BUFFER, renderer->sprite_vbo);
    glBufferData(GL_ARRAY_BUFFER, sprite_vertices_max, renderer->sprite_vertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    char *sprite_vsh_src = sc_read("shaders/sprite.vsh");
    char *sprite_fsh_src = sc_read("shaders/sprite.fsh");
    sc_uint32 sprite_vsh = load_shader(sprite_vsh_src, GL_VERTEX_SHADER);
    sc_uint32 sprite_fsh = load_shader(sprite_fsh_src, GL_FRAGMENT_SHADER);
    free(sprite_vsh_src);
    free(sprite_fsh_src);

    renderer->sprite_shader = glCreateProgram();
    if (!renderer->sprite_shader || !sprite_fsh || !sprite_vsh) {
        fprintf(stderr, "Failed to initialize shader.");
        exit(EXIT_FAILURE);
    }

    glAttachShader(renderer->sprite_shader, sprite_vsh);
    glAttachShader(renderer->sprite_shader, sprite_fsh);
    glLinkProgram(renderer->sprite_shader);

    int success;
    glGetProgramiv(renderer->sprite_shader, GL_LINK_STATUS, &success);
    if(!success) {
        fprintf(stderr, "Shader program linkage failed.");
        exit(EXIT_FAILURE);
    }

    glDeleteShader(sprite_fsh);
    glDeleteShader(sprite_vsh);

    return renderer;
}

void SC_FASTCALL scRenderer_free(scRenderer *renderer) {
    if (!renderer) return;

    glDeleteVertexArrays(1, &renderer->sprite_vao);
    glDeleteBuffers(1, &renderer->sprite_vbo);
    free(renderer->sprite_vertices);

    free(renderer->gpu_info.name);
    free(renderer->gpu_info.brand);
    SDL_GL_DeleteContext(renderer->ctx);
    free(renderer);
}

void SC_FASTCALL scRenderer_add_sprite(scRenderer *renderer, scSprite *sprite) {
    sc_real left = sprite->x - 15.0;
    sc_real right = sprite->x + 15.0;
    sc_real top = sprite->y - 15.0;
    sc_real bottom = sprite->y + 15.0;

    scratch_to_screen_space(&left, &top, renderer->viewport_wh, renderer->viewport_hh);
    scratch_to_screen_space(&right, &bottom, renderer->viewport_wh, renderer->viewport_hh);

    normalize_coords(&left, &top, renderer->viewport_w, renderer->viewport_h);
    normalize_coords(&right, &bottom, renderer->viewport_w, renderer->viewport_h);

    renderer->sprite_vertices[renderer->sprite_vertices_n] = left;
    renderer->sprite_vertices[renderer->sprite_vertices_n+1] = top;
    renderer->sprite_vertices[renderer->sprite_vertices_n+2] = left;
    renderer->sprite_vertices[renderer->sprite_vertices_n+3] = bottom;
    renderer->sprite_vertices[renderer->sprite_vertices_n+4] = right;
    renderer->sprite_vertices[renderer->sprite_vertices_n+5] = bottom;
    renderer->sprite_vertices[renderer->sprite_vertices_n+6] = right;
    renderer->sprite_vertices[renderer->sprite_vertices_n+7] = bottom;
    renderer->sprite_vertices[renderer->sprite_vertices_n+8] = right;
    renderer->sprite_vertices[renderer->sprite_vertices_n+9] = top;
    renderer->sprite_vertices[renderer->sprite_vertices_n+10] = left;
    renderer->sprite_vertices[renderer->sprite_vertices_n+11] = top;
    renderer->sprite_vertices_n += 12;

    renderer->sprite_vao_n += 6;
}

void SC_FASTCALL scRenderer_render(scRenderer *renderer) {
    glUseProgram(renderer->sprite_shader);

    glBindVertexArray(renderer->sprite_vao);
    glDrawArrays(GL_TRIANGLES, 0, renderer->sprite_vao_n);
    glBindVertexArray(0);
}