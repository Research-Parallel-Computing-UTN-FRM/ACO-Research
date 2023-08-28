#include <stdlib.h>
#include <stdio.h>
#include "system.h"
#include "utils.h"
#include <errno.h>
#include <time.h>

int main(int argc, char **argv)
{
    char *p;
    int n_cities = strtol(argv[1], &p, 10);
    int n_args = strtol(argv[2], &p, 10);
    float alpha = atof(argv[3]);
    float beta = atof(argv[4]);
    float evaporation_rate = atof(argv[5]);
    int cycles = strtol(argv[6], &p, 10);

    int *best_sols = malloc(sizeof(int) * cycles);

    int next;
    ant_system *s;
    s = new_system(n_cities, n_args, alpha, beta, evaporation_rate, cycles);
    if (s == NULL)
    {
        printf("FAILED");
        return 0;
    }

    // printf("FIRST INITIALIZATION");
    // print_system(s);
    clock_t start, end;
    start = clock();

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
                // print_system(s);
            }
        }
        // print_system(s);
        best_solution(s, best_sols, c);
        update_pheromones(s);
        reset_tabu_list(s);
    }
    end = clock();

    printf("%ld\n", end - start);

    // print_int_array(best_sols, cycles);
    // printf("END OF ITERATIONS \n");
    // print_system(s);

    free_system(s);

    return 0;
}
