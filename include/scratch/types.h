/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/


#ifndef _SCRATCH_TYPES_H
#define _SCRATCH_TYPES_H

#include <stdint.h>
#include <math.h>


// TODO: Option to change sc_real precision in compile-time.
typedef double sc_real;
#define sc_sin sin
#define sc_cos cos
#define sc_fmod fmod
#define sc_round round
#define sc_fabs fabs

typedef int8_t sc_int8;
typedef int16_t sc_int16;
typedef int32_t sc_int32;
typedef int64_t sc_int64;
typedef uint8_t sc_uint8;
typedef uint16_t sc_uint16;
typedef uint32_t sc_uint32;
typedef uint64_t sc_uint64;



#endif