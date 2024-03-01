/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/


#ifndef _SCRATCH_VARIABLE_H
#define _SCRATCH_VARIABLE_H

#include "scratch/_scratch.h"
#include "scratch/sprite.h"


typedef enum {
    scVariableType_REAL,
    scVariableType_INTEGER,
    scVariableType_STRING
} scVariableType;

typedef struct {
    scVariableType type;
    union {
        sc_real value_real;
        sc_int64 value_int;
        char *value_str;
    };
} scVariable;

#define SC_VARIABLE_REAL(x) ((scVariable){.type=scVariableType_REAL, .value_real=(x)})
#define SC_VARIABLE_INTEGER(x) ((scVariable){.type=scVariableType_INTEGER, .value_int=(x)})


#endif