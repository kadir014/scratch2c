/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/

#include "scratch/core/array.h"


scArray *scArray_new() {
    scArray *array = (scArray *)malloc(sizeof(scArray));
    if (!array) return NULL;

    array->size = 0;
    array->max = 0;
    array->data = (void **)malloc(sizeof(void *));
    if (!array->data) {
        free(array);
        return NULL;
    }

    return array;
}

void scArray_free(scArray *array) {
    free(array->data);
    array->data = NULL;
    array->size = 0;
    free(array);
}

void scArray_free_each(scArray *array, void (free_func)(void *)) {
    for (size_t i = 0; i < array->size; i++)
        free_func(array->data[i]);
}

void scArray_add(scArray *array, void *elem) {
    // Only reallocate when max capacity is reached
    if (array->size == array->max) {
        array->size++;
        array->max++;
        array->data = (void **)realloc(array->data, array->size * sizeof(void *));
    }
    else {
        array->size++;
    }

    array->data[array->size - 1] = elem;
}

void *scArray_pop(scArray *array, size_t index) {
    for (size_t i = 0; i < array->size; i++) {
        if (i == index) {
            array->size--;
            void *elem = array->data[i];

            array->data[i] = array->data[array->size];
            array->data[array->size] = NULL;

            return elem;
        }
    }

    return NULL;
}

size_t scArray_remove(scArray *array, void *elem) {
    for (size_t i = 0; i < array->size; i++) {
        if (array->data[i] == elem) {
            array->size--;

            array->data[i] = array->data[array->size];
            array->data[array->size] = NULL;

            return i;
        }
    }

    return -1;
}

void scArray_clear(scArray *array, void (free_func)(void *)) {
    /*
        We can set array->max to 0 and reallocate but
        not doing it might be more efficient for the developer
        since they will probably fill the array up again.
        Maybe a separate parameter for this?
    */

    if (array->size == 0) return;
   
    if (!free_func) {
        while (array->size > 0) {
            scArray_pop(array, 0);
        }
    }

    else {
        while (array->size > 0) {
            free_func(scArray_pop(array, 0));
        }
    }
}