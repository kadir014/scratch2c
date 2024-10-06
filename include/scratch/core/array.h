/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/


#ifndef _SCRATCH_ARRAY_H
#define _SCRATCH_ARRAY_H

#include "scratch/_scratch.h"


/**
 * @brief Type-generic dynamically growing array implementation.
 */
typedef struct {
    size_t size; /**< Length of the array. */
    size_t max; /**< Maximum size the array ever reached, this is basically the size of the array on HEAP. */
    void **data; /**< Array of void pointers. */
} scArray;

/**
 * @brief Create new array.
 * 
 * @return scArray *
 */
scArray *scArray_new();

/**
 * @brief Free array.
 * 
 * @param array Array to free
 */
void scArray_free(scArray *array);

/**
 * @brief Free each element of array.
 * 
 * @param array Array
 * @param free_func Free function
 */
void scArray_free_each(scArray *array, void (free_func)(void *));

/**
 * @brief Add new element to array.
 * 
 * @param array Array to append to
 * @param elem Void pointer to element
 */
void scArray_add(scArray *array, void *elem);

/**
 * @brief Remove element by index from array and return the element. Returns `NULL` if failed.
 * 
 * @note The array is not sorted after removal, meaning the array gets slightly randomized every remove call.
 * 
 * @param array Array
 * @param index Index of element to remove
 * @return void *
 */
void *scArray_pop(scArray *array, size_t index);

/**
 * @brief Remove element from array and return the index. Returns `-1` if failed.
 * 
 * @note The array is not sorted after removal, meaning the array gets slightly randomized every remove call.
 * 
 * @param array Array
 * @param elem Element to remove
 * @return size_t
 */
size_t scArray_remove(scArray *array, void *elem);

/**
 * @brief Clear the array.
 * 
 * Elements are not freed if `NULL` is passed as freeing function.
 * 
 * @param array Array
 * @param free_func Free function
 */
void scArray_clear(scArray *array, void (free_func)(void *));


#endif