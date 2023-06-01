#include <stddef.h>

struct Array
{
    int *array;
    int used;
    int size;
};
typedef struct Array Array;

Array *initArray(int initialSize);
void insertArray(Array *a, int element);
void freeArray(Array *a);
