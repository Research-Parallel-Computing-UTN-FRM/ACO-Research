#include <stdlib.h>
#include <stdio.h>
#include "system.h"
#include "util.h"

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

    // Run 1 cycle
    next_city(s, 0, 0);

    // for (int ant = 0; ant < s->n_ants; ant++)
    // {
    //     for (int iter = 0; iter < s->n_cities; iter++)
    //     {
    //         printf("[%d] [%d] \n", ant, iter);
    //         printf("DIDNT ENTER \n");
    //         next = next_city(s, ant, iter);
    //         // if (next == -1)
    //         // {
    //         //     printf("ERROR DURING EXCECUTION, ant: %d  iter: %d \n ", ant, iter);
    //         //     return 64;
    //         // }
    //         // move_to_city(s, ant, iter, next);
    //     }
    // }
    print_system(s);

    // // set_city_as_visited(s, 1);
    // update_pheromone(s);
    // print_system(s);

    // printf(" asd %f \n", city_probability(s, 0, 2));
    // print_int_array(visited_cities(s, 3), 5);

    // printf("%d \n", is_value_in_array(s->cities, s->n_cities, 4));
    // printf("\n\n");
    // print_int_matrix(s->list_tabu_list, s->n_ants, s->n_cities);
    // printf("\n\n");
    // not_visited_cities(s, 3);
    free(s);
    return 0;
}
