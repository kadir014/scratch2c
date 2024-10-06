/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/


#ifndef _SCRATCH_SENSING_H
#define _SCRATCH_SENSING_H

#include "scratch/_scratch.h"
#include "scratch/engine/sprite.h"


typedef enum {
    scCurrentDateArg_YEAR,
    scCurrentDateArg_MONTH,
    scCurrentDateArg_DATE,
    scCurrentDateArg_DAY_OF_WEEK,
    scCurrentDateArg_HOUR,
    scCurrentDateArg_MINUTE,
    scCurrentDateArg_SECOND
} scCurrentDateArg;

/**
 * @brief Return the current date information. Use the scCurrentDateArg enum.
 * 
 * @param date_arg Date argument enum
 * 
 * @return unsigned int 
 */
static inline unsigned int SC_FASTCALL sc_curent_date(
    scCurrentDateArg date_arg
) {
    time_t now;
    time(&now);
    struct tm *time_info = localtime(&now);

    switch (date_arg) {
        case scCurrentDateArg_YEAR:
            return time_info->tm_year + 1900; // Since 1900

        case scCurrentDateArg_MONTH:
            return time_info->tm_mon + 1; // 0-indexed

        case scCurrentDateArg_DATE:
            return time_info->tm_mday;

        case scCurrentDateArg_DAY_OF_WEEK:
            return time_info->tm_wday + 1; // Sunday-based weekdays?

        case scCurrentDateArg_HOUR:
            return time_info->tm_hour;

        case scCurrentDateArg_MINUTE:
            return time_info->tm_min;

        case scCurrentDateArg_SECOND:
            return time_info->tm_sec;

        default:
            // Raise error?
            return 0;
    }
}

/**
 * @brief Return the days since year 2000.
 * 
 * @return double 
 */
static inline double SC_FASTCALL sc_days_since_2000() {
    #if defined(SC_PLATFORM_WINDOWS)

        double sec_per_day = 24.0 * 60.0 * 60.0;

        SYSTEMTIME start_tm;
        start_tm.wYear = 2000;
        start_tm.wMonth = 1;
        start_tm.wDay = 1;
        start_tm.wHour = 0;
        start_tm.wMinute = 0;
        start_tm.wSecond = 0;
        start_tm.wMilliseconds = 0;

        SYSTEMTIME now_tm;
        GetSystemTime(&now_tm);

        FILETIME ft_start, ft_now;
        SystemTimeToFileTime(&start_tm, &ft_start);
        SystemTimeToFileTime(&now_tm, &ft_now);

        ULARGE_INTEGER start, now;
        start.LowPart = ft_start.dwLowDateTime;
        start.HighPart = ft_start.dwHighDateTime;
        now.LowPart = ft_now.dwLowDateTime;
        now.HighPart = ft_now.dwHighDateTime;

        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        double frequency_d = (double)frequency.QuadPart;

        double start_secs = (double)start.QuadPart / frequency_d;
        double now_secs = (double)now.QuadPart / frequency_d;

        return (now_secs - start_secs) / sec_per_day;

    #else

        // double ns_per_day = 24.0 * 60 * 60 * 1000000000.0;
        // struct tm start = {0};
        // start.tm_year = 100; // Since 1900, so 100 -> 2000
        // start.tm_mon = 0;
        // start.tm_mday = 1;

        // struct timespec now;
        // clock_gettime(CLOCK_, &now);

        // double sec = now.tv_sec;
        // double ns = now.tv_nsec;

        // double ns_since_start = (sec - mktime(&start)) * 1000000000.0 + ns;
        // return ns_since_start / ns_per_day;

        return 0.0;

    #endif
}

/**
 * @brief Return the username. Currently returns an empty string.
 * 
 * @return char * 
 */
static inline char *SC_FASTCALL sc_username() {
    return "";
}


#endif