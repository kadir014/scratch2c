/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/


#ifndef _SCRATCH_RENDERER_H
#define _SCRATCH_RENDERER_H


#include "scratch/_scratch.h"
#include "scratch/engine/sprite.h"


typedef enum {
    scRendererGLProfile_CORE,
    scRendererGLProfile_COMPATIBILITY,
    scRendererGLProfile_ES
} scRendererGLProfile;

static inline char *SC_FASTCALL scRendererGLProfile_as_str(
    scRendererGLProfile profile
) {
    switch (profile) {
        case scRendererGLProfile_CORE:
            return "Core";

        case scRendererGLProfile_COMPATIBILITY:
            return "Compatibility";

        default:
        case scRendererGLProfile_ES:
            return "ES";
    }
}

typedef struct {
    int major;
    int minor;
} scRendererGLVersion;

typedef struct {
    char *name;
    char *brand;
} scRendererGPUInfo;

typedef struct {
    SDL_GLContext ctx;
    scRendererGLProfile gl_profile;
    scRendererGLVersion gl_version;
    scRendererGPUInfo gpu_info;

    sc_real viewport_w;
    sc_real viewport_h;
    sc_real viewport_wh;
    sc_real viewport_hh;

    sc_uint32 sprite_vao;
    size_t sprite_vao_n;
    float *sprite_vertices;
    size_t sprite_vertices_n;
    sc_uint32 sprite_vbo;
    sc_uint32 sprite_uvbo;
    sc_uint32 sprite_shader;
} scRenderer;

scRenderer *SC_FASTCALL scRenderer_new(
    SDL_Window *window,
    sc_real viewport_w,
    sc_real viewport_h
);

void SC_FASTCALL scRenderer_free(scRenderer *renderer);

void SC_FASTCALL scRenderer_add_sprite(scRenderer *renderer, scSprite *sprite);

void SC_FASTCALL scRenderer_render(scRenderer *renderer);


#endif