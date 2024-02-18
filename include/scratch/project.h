/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/


#ifndef _SCRATCH_PROJECT_H
#define _SCRATCH_PROJECT_H

#include "scratch/_scratch.h"


typedef struct {
    const char *scratch_ver;
    const char *vm_ver;
    const char *user_agent;
} scProjectMetadata;


typedef struct {
    sc_uint16 stage_width;
    sc_uint16 stage_height;
    scProjectMetadata metadata;
} scProject;

static const scProject scProject_default = {
    .stage_width=480,
    .stage_height=360,
    .metadata={
        .scratch_ver="",
        .vm_ver="",
        .user_agent=""
    }
};


#endif