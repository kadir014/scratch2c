/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/


#ifndef _SCRATCH_RENDERER_H
#define _SCRATCH_RENDERER_H


#include "scratch/_scratch.h"


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
} scRenderer;

scRenderer *SC_FASTCALL scRenderer_new(SDL_Window *window);

void SC_FASTCALL scRenderer_free(scRenderer *renderer);



#endif