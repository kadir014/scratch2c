/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/


#ifndef _SCRATCH_PLATFORM_H
#define _SCRATCH_PLATFORM_H


/*

    OS/Platform identification

*/

#if defined(__EMSCRIPTEN__) || defined(__wasi__)

    #define SC_PLATFORM_WEB

#elif defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)

    #define SC_PLATFORM_WINDOWS
    
#elif defined(__linux__) && !defined(__ANDROID__)

    #define SC_PLATFORM_LINUX

#elif defined(__APPLE__) || defined(__MACH__)

    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE

        #define SC_PLATFORM_IOS

    #elif TARGET_OS_MAC

        #define SC_PLATFORM_MACOS

    #endif

#elif defined(__ANDROID__)

    #define SC_PLATFORM_ANDROID

#elif defined(__unix__) || defined(__unix)

    #define SC_PLATFORM_UNIX

#else

    #define SC_PLATFORM_UNKNOWN

#endif


/*

    Compiler identification

*/

#if defined(__GNUC__) || defined(__MINGW32__) || defined(__MINGW64__)

    #define SC_COMPILER_GCC

#elif defined(__clang__)

    #define SC_COMPILER_CLANG

#elif defined(_MSC_VER) || defined(_MSC_FULL_VER) || defined(_MSVC_LANG)

    #define SC_COMPILER_MSVC

#else

    #define SC_COMPILER_UNKNOWN

#endif


/*

    Other platform dependant definitions

*/

#if defined(SC_COMPILER_GCC) || defined(SC_COMPILER_CLANG)

    #define SC_FASTCALL __attribute__((fastcall))

#elif defined(SC_COMPILER_MSVC)

    #define SC_FASTCALL __fastcall

#else

    #define SC_FASTCALL

#endif


#endif