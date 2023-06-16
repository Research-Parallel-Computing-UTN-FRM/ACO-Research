#include <stdlib.h>
#include <stdio.h>
#include "system.h"
#include "utils.h"

int main()
{
    int next;
    ant_system *s;
    s = new_system(8, 3, 1, 1, 0.5, 2);
    if (s == NULL)
    {
        printf("FAILED");
        return 0;
    }

    printf("FIRST INITIALIZATION");
    print_system(s);

    for (int c = 0; c < s->n_cycles; c++)
    {
        // A single cycle
        for (int iter = 0; iter < (s->n_cities - 1); iter++)
        {
            // Iter initializes as 1 because the first city (iter 0)
            // is defined during system initialization
            for (int ant = 0; ant < s->n_ants; ant++)
            {
                next = next_city(s, ant, iter);
                move_to_city(s, ant, iter + 1, next);
                print_system(s);
            }
            
        }
        print_system(s);
        best_solution(s);
        update_pheromones(s);
        reset_tabu_list(s);
    }

    printf("END OF ITERATIONS \n");
    print_system(s);

    free_system(s);

    return 0;
}
