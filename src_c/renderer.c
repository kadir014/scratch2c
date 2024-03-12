/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/

#include "scratch/renderer.h"


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


scRenderer *SC_FASTCALL scRenderer_new(SDL_Window *window) {
    scRenderer *renderer = (scRenderer *)malloc(sizeof(scRenderer));
    if (!renderer) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return NULL;
    }

    renderer->ctx = SDL_GL_CreateContext(window);
    if (!renderer->ctx) {
        fprintf(stderr, SDL_GetError());
        return NULL;
    }
    SDL_GL_MakeCurrent(window, renderer->ctx);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
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

    return renderer;
}

void SC_FASTCALL scRenderer_free(scRenderer *renderer) {
    if (!renderer) return;

    free(renderer->gpu_info.name);
    free(renderer->gpu_info.brand);
    SDL_GL_DeleteContext(renderer->ctx);
    free(renderer);
}