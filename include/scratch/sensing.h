/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/


#ifndef _SCRATCH_SENSING_H
#define _SCRATCH_SENSING_H

#include "scratch/_scratch.h"
#include "scratch/sprite.h"


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
 * Category: Sensing
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
            return time_info->tm_wday;

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
 * @brief Return the days since year 2000 in milliseconds.
 * 
 * Category: Sensing
 * 
 * @return double 
 */
static inline double SC_FASTCALL sc_days_since_2000() {
    double ms_per_day = 24 * 60 * 60 * 1000;

    struct tm start = {0};
    start.tm_year = 100; // Since 1900, so 100 represents 2000
    start.tm_mon = 0; // 0-indexed
    start.tm_mday = 1;

    time_t now;
    time(&now);
    struct tm *today = localtime(&now);
    
    double ms_since = difftime(mktime(today), mktime(&start)) * 1000;
    return ms_since / ms_per_day;
}

/**
 * @brief Return the username. Currently returns an empty string.
 * 
 * Category: Sensing
 * 
 * @return char* 
 */
static inline char * SC_FASTCALL sc_username() {
    return "";
}


#endif