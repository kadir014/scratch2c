/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/


#ifndef _SCRATCH_MAIN_H
#define _SCRATCH_MAIN_H


#define _SC_STRINGIFY(x) #x
#define SC_STRINGIFY(x) _SC_STRINGIFY(x)

// Version in MAJOR.MINOR.PATCH format
#define SC_VERSION_MAJOR 0
#define SC_VERSION_MINOR 0
#define SC_VERSION_PATCH 1
// Version string
#define SC_VERSION_STRING                  \
        SC_STRINGIFY(SC_VERSION_MAJOR) "." \
        SC_STRINGIFY(SC_VERSION_MINOR) "." \
        SC_STRINGIFY(SC_VERSION_PATCH)


#include "scratch/_scratch.h"
#include "scratch/core/math.h"
#include "scratch/core/array.h"
#include "scratch/core/prng.h"

#include "scratch/engine/engine.h"
#include "scratch/engine/renderer.h"
#include "scratch/engine/project.h"
#include "scratch/engine/sprite.h"

#include "scratch/blocks/variable.h"
#include "scratch/blocks/motion.h"
#include "scratch/blocks/looks.h"
#include "scratch/blocks/control.h"
#include "scratch/blocks/sensing.h"
#include "scratch/blocks/operators.h"


#endif