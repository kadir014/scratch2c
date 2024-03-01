/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/


#ifndef _SCRATCH_MATH_H
#define _SCRATCH_MATH_H

#include "scratch/_scratch.h"


#define SC_PI 3.141592653589793238462643383279502884
#define SC_RAD_TO_DEG 57.295779513082322864647721871733665466
#define SC_DEG_TO_RAD 0.017453292519943295474371680597869272


/**
 * @brief Return random sc_real in range [lower, higher).
 * 
 * @param lower Lower limit
 * @param higher Higher limit
 * @return sc_float 
 */
static inline sc_real sc_random(sc_real lower, sc_real higher) {
    sc_real normal = rand() / (float)RAND_MAX;
    return lower + normal * (higher - lower);
}


#endif