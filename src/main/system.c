#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "initialize.h"
#include "system.h"
#include "util.h"

ant_system *malloc_system(int n_cities, int n_ants)
{
    // Malloc the  struct
    ant_system *system = (struct ant_system *)malloc(sizeof(struct ant_system));

    // Malloc the cities list
    system->cities = (int *)malloc(sizeof(int) * n_cities);

    // Malloc n_ants x n_cities array
    // Allocate first the rows (each ant will have one row with its path)
    system->list_tabu_list = (int **)malloc(sizeof(int *) * n_ants);
    for (int i = 0; i < n_ants; i++)
    {
        // Allocate the columns for each row, the column have the
        // node visited in the nth step of the path
        system->list_tabu_list[i] = (int *)malloc(sizeof(int) * n_cities);
    }

    // Malloc n_cities x n_cities matrix
    system->cities_distances = (int **)malloc(sizeof(int *) * n_cities);
    system->pheromones = (int **)malloc(sizeof(int *) * n_cities);
    for (int i = 0; i < n_cities; i++)
    {
        system->cities_distances[i] = (int *)malloc(sizeof(int) * n_cities);
        system->pheromones[i] = (int *)malloc(sizeof(int) * n_cities);
    }

    return system;
}

ant_system *new_system(int n_cities, int n_ants, float alpha, float beta, float evaporation_rate)
{
    ant_system *system;

    system = malloc_system(n_cities, n_ants);
    if (system == NULL)
        return NULL;

    system->n_cities = n_cities;
    system->n_ants = n_ants;
    system->alpha = alpha;
    system->beta = beta;
    system->evaporation_rate = evaporation_rate;
    system->cities = initialize_cities_list(system->n_cities);
    system->cities_distances = initialize_distances(n_cities);
    system->pheromones = initialize_pheromones(n_cities);
    system->list_tabu_list = initialize_paths(n_ants, n_cities);

    return system;
}

// Increments pheromones of the best solution by 1
void update_pheromone(ant_system *system)
{
    int limit = system->n_cities;
    for (int i = 0; i < limit; i++)
    {
        for (int j = 0; j < limit; j++)
        {
            system->pheromones[i][j]++;
        }
    }
}

// current -> The index of the current city
// target -> The index of the target city
double city_probability(ant_system *s, int current, int target)
{
    int distance = s->cities_distances[current][target];
    int pheromone = s->pheromones[current][target];

    float inverse_distance = 1.0f / distance;
    double probability = pow(pheromone, s->alpha) * pow(inverse_distance, s->beta);
    return probability;
}

int *visited_cities(ant_system *s, int ant_number)
{
    return s->list_tabu_list[ant_number];
}

Array *not_visited_cities(ant_system *s, int ant_number)
{
    // The dynamic array is necessary because i dont know (easily)
    // the amount of cities not visited on the iteration

    // If the number of not visited cities can be obtained
    // a simple array can be used, and it should be faster
    Array *a;

    a = initArray(1);
    printf("Ant number: %d \n", ant_number);
    print_int_array(s->list_tabu_list[ant_number], s->n_cities);
    for (int i = 0; i < s->n_cities; i++)
    {
        if (is_value_in_array(visited_cities(s, ant_number), s->n_cities, i) == 0)
        {
            insertArray(a, i);
        }
    }
    printf("array size: %ld \n", a->size);
    printf("Not visited citites: ");
    print_int_array(a->array, a->size);
    return a;
}

int current_city(ant_system *s, int ant_number, int iter)
{
    return s->list_tabu_list[ant_number][iter];
}

// Returns the number of the next city to move
int next_city(ant_system *s, int ant_number, int iter)
{
    int current, next_idx, next;
    double *probabilities;
    Array *not_visited;

    not_visited = not_visited_cities(s, ant_number);
    probabilities = (double *)malloc(sizeof(int) * not_visited->size);

    for (int i = 0; i < not_visited->size; i++)
    {
        current = current_city(s, ant_number, iter);
        probabilities[i] = city_probability(s, current, not_visited->array[i]);
    }

    printf("Probabilities: \n");
    for (int i = 0; i < not_visited->size; i++)
    {
        printf("City:%d P:%f\n", not_visited->array[i], probabilities[i]);
    }
    // Pick based on the discrete distribution stored inside probabilities
    next_idx = random_from_discrete_distribution(probabilities, not_visited->size);
    if (next_idx == -1)
        return -1;
    next = not_visited->array[next_idx];

    freeArray(not_visited);
    free(probabilities);

    return next;
}

// iter -> The movement number.
// i.e The first time the ant moves (iter = 1),
// will set tabu_list[1] (0 is already defined on initialization) when moves
// from the initial city to the first target
void move_to_city(ant_system *s, int ant, int iter, int city)
{
    s->list_tabu_list[ant][iter] = city;
}
