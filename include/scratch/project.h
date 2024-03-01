/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/


#ifndef _SCRATCH_PROJECT_H
#define _SCRATCH_PROJECT_H

#include "scratch/_scratch.h"
#include "scratch/sprite.h"


typedef struct {
    const char *scratch_ver;
    const char *vm_ver;
    const char *user_agent;
} scProjectMetadata;


typedef struct {
    sc_uint16 stage_width;
    sc_uint16 stage_height;
    sc_real stage_width2;
    sc_real stage_height2;
    scProjectMetadata metadata;
    scSprite *targets;
} scProject;

static const scProject scProject_default = {
    .stage_width=480,
    .stage_height=360,
    .stage_width2=240.0,
    .stage_height2=180.0,
    .metadata={
        .scratch_ver="",
        .vm_ver="",
        .user_agent=""
    },
    .targets=NULL
};


#endif