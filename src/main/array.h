#include <stddef.h>

struct Array
{
    int *array;
    size_t used;
    size_t size;
};
typedef struct Array Array;

Array *initArray(size_t initialSize);
void insertArray(Array *a, int element);
void freeArray(Array *a);
