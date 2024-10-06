/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/


#ifndef _SCRATCH_VARIABLE_H
#define _SCRATCH_VARIABLE_H

#include "scratch/_scratch.h"


typedef enum {
    scVariableType_REAL,
    scVariableType_STRING
} scVariableType;

typedef struct {
    scVariableType type;
    union {
        sc_real value_real;
        char *value_str;
    };
} scVariable;

#define SC_VARIABLE_REAL(x) ((scVariable){.type=scVariableType_REAL, .value_real=(x)})
#define SC_VARIABLE_STRING(x) ((scVariable){.type=scVariableType_STRING, .value_str=(x)})

static inline scVariable SC_FASTCALL scVariable_cast(scVariable a, scVariableType type) {
    if (a.type == scVariableType_REAL && type == scVariableType_REAL) {
        return a;
    }
    else if (a.type == scVariableType_REAL && type == scVariableType_STRING) {
        // TODO: CAST REAL TO STRING
        a.type = type;
        a.value_str = "";
        return a;
    }
    else if (a.type == scVariableType_STRING && type == scVariableType_REAL) {
        // TODO: CAST STRING TO REAL
        a.type = type;
        a.value_real = 0.0;
        return a;
    }
    else if (a.type == scVariableType_STRING && type == scVariableType_STRING) {
        return a;
    }

    return SC_VARIABLE_REAL(0.0); // -Wreturn-type
}

static inline void SC_FASTCALL scVariable_assign(scVariable *a, scVariable b) {
    if (b.type == scVariableType_REAL) {
        a->type = scVariableType_REAL;
        a->value_real = b.value_real;
    }
    else if (b.type == scVariableType_STRING) {
        a->type = scVariableType_STRING;
        a->value_str = b.value_str;
    }
}


#endif