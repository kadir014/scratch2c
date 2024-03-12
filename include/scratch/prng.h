/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/


#ifndef _SCRATCH_PRNG_H
#define _SCRATCH_PRNG_H

#include "scratch/_scratch.h"


/**
 * @brief Pseudo-random number generator algorithm.
 */
typedef enum {
    scPRNGAlg_DEFAULT, /**< Standard rand implementation available on the platform. */
    scPRNGAlg_XORSHIFT128P /**< Xorshift128+ algorithm. */
} scPRNGAlg;

typedef union {
    char empty;
    sc_uint64 xorshift128p[2];
} scPRNGState;

/**
 * @brief Initialize PRNG.
 * 
 * @return scPRNGState 
 */
static inline scPRNGState SC_FASTCALL scPRNG_init(scPRNGAlg alg) {
    scPRNGState state;

    switch (alg) {
        default:
        case scPRNGAlg_DEFAULT:
            srand(time(NULL));
            state.empty = 0;
            return state;

        case scPRNGAlg_XORSHIFT128P:
            // TODO: Better seeding?
            state.xorshift128p[0] = time(NULL);
            state.xorshift128p[1] = time(NULL) * 0x21294E38D;
            return state;
    }
}


static inline sc_uint64 SC_FASTCALL sc_xorshift128p(scPRNGState *state) {
    // https://en.wikipedia.org/wiki/Xorshift#xorshift+

    sc_uint64 t = state->xorshift128p[0];
    sc_uint64 const s = state->xorshift128p[1];
    state->xorshift128p[0] = s;
    t ^= t << 23;
    t ^= t >> 18;
    t ^= s ^ (s >> 5);
    state->xorshift128p[1] = t;
    return t + s;
}


#endif