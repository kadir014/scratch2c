/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/


#ifndef _SCRATCH_OPERATORS_H
#define _SCRATCH_OPERATORS_H

#include "scratch/_scratch.h"
#include "scratch/variable.h"
#include "scratch/math.h"


#define _SC_ARITHMETIC(a, b, op) \
    SC_VARIABLE_REAL(scVariable_cast(a, scVariableType_REAL).value_real op scVariable_cast(b, scVariableType_REAL).value_real)

#define sc_op_add(a, b) _SC_ARITHMETIC((a), (b), +)
#define sc_op_sub(a, b) _SC_ARITHMETIC((a), (b), -)
#define sc_op_mul(a, b) _SC_ARITHMETIC((a), (b), *)
#define sc_op_div(a, b) _SC_ARITHMETIC((a), (b), /)
#define sc_op_gt(a, b) _SC_ARITHMETIC((a), (b), >)
#define sc_op_lt(a, b) _SC_ARITHMETIC((a), (b), <)
#define sc_op_eq(a, b) _SC_ARITHMETIC((a), (b), ==)
#define sc_op_and(a, b) _SC_ARITHMETIC((a), (b), &&)
#define sc_op_or(a, b) _SC_ARITHMETIC((a), (b), ||)

static inline scVariable SC_FASTCALL sc_op_rand(scVariable a, scVariable b) {
    return SC_VARIABLE_REAL(sc_random(scVariable_cast(a, scVariableType_REAL).value_real, scVariable_cast(b, scVariableType_REAL).value_real));
}

static inline scVariable SC_FASTCALL sc_op_not(scVariable a) {
    return SC_VARIABLE_REAL(!(scVariable_cast(a, scVariableType_REAL).value_real));
}

static inline scVariable SC_FASTCALL sc_op_join(scVariable a, scVariable b) {
    return SC_VARIABLE_STRING("");
}

static inline scVariable SC_FASTCALL sc_op_index(scVariable a, scVariable b) {
    return SC_VARIABLE_STRING(((char[2]){(char)(scVariable_cast(b, scVariableType_STRING).value_str[(size_t)(scVariable_cast(a, scVariableType_REAL).value_real)]), '\0'}));
}

static inline scVariable SC_FASTCALL sc_op_len(scVariable a) {
    return SC_VARIABLE_REAL((sc_real)strlen(scVariable_cast(a, scVariableType_STRING).value_str));
}

static inline scVariable SC_FASTCALL sc_op_in(scVariable a, scVariable b) {
    return SC_VARIABLE_REAL(0.0);
}

static inline scVariable SC_FASTCALL sc_op_mod(scVariable a, scVariable b) {
    return SC_VARIABLE_REAL(sc_fmod(scVariable_cast(a, scVariableType_REAL).value_real, scVariable_cast(b, scVariableType_REAL).value_real));
}

static inline scVariable SC_FASTCALL sc_op_round(scVariable a) {
    return SC_VARIABLE_REAL(sc_round(scVariable_cast(a, scVariableType_REAL).value_real));
}

static inline scVariable SC_FASTCALL sc_op_abs(scVariable a) {
    return SC_VARIABLE_REAL(sc_fabs(scVariable_cast(a, scVariableType_REAL).value_real));
}


#endif