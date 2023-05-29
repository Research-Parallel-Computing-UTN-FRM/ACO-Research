#include <stdlib.h>
#include <stdio.h>
#include "system.h"
#include "utils.h"

int main()
{
    int next;
    ant_system *s;
    s = new_system(3, 3, 1, 1, 0.5);
    if (s == NULL)
    {
        printf("FAILED");
        return 0;
    }

    print_system(s);

    // A cycle test
    for (int ant = 0; ant < s->n_ants; ant++)
    {
        // Iter initializes as 1 because the first city
        // are defined during system initialization
        for (int iter = 1; iter < s->n_cities; iter++)
        {
            next = next_city(s, ant, iter);
            move_to_city(s, ant, iter, next);
        }
    }

    print_system(s);

    free(s);

    return 0;
}
