#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "system.h"

int main()
{

    ant_system *s;
    s = new_system(3, 100, 0.5, 0.5);
    if (s == NULL)
    {
        printf("FAILED");
        return 0;
    }

    print_system(s);

    free(s);
    return 0;
}
