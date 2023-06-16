#include "array.h"
#include <stdlib.h>

Array *initArray(int initialSize)
{
    Array *a = (struct Array *)malloc(sizeof(struct Array));
    a->array = malloc(initialSize * sizeof(int));
    a->used = 0;
    a->size = initialSize;

    return a;
}

void insertArray(Array *a, int element)
{
    // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
    // Therefore a->used can go up to a->size
    if (a->used == a->size)
    {
        a->size += 1;
        a->array = realloc(a->array, a->size * sizeof(int));
    }
    a->array[a->used++] = element;
}

void freeArray(Array *a)
{
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}