#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

#include "initialize.h"
#include "system.h"
#include "utils.h"

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
    system->pheromones = (double **)malloc(sizeof(double *) * n_cities);
    for (int i = 0; i < n_cities; i++)
    {
        system->cities_distances[i] = (int *)malloc(sizeof(int) * n_cities);
        system->pheromones[i] = (double *)malloc(sizeof(double) * n_cities);
    }

    return system;
}

void free_system(ant_system *s)
{
    for (int i = 0; i < s->n_cities; i++)
    {
        free(s->cities_distances[i]);
        free(s->pheromones[i]);
    }
    free(s->cities_distances);
    free(s->pheromones);

    for (int i = 0; i < s->n_ants; i++)
    {
        free(s->list_tabu_list[i]);
    }
    free(s->list_tabu_list);
    free(s->cities);

    free(s);
}

ant_system *new_system(int n_cities, int n_ants, float alpha, float beta, float evaporation_rate, int n_cycles)
{
    ant_system *system;

    system = malloc_system(n_cities, n_ants);
    if (system == NULL)
        return NULL;

    // Constants for the system
    system->n_cycles = n_cycles;
    system->n_cities = n_cities;
    system->n_ants = n_ants;
    system->alpha = alpha;
    system->beta = beta;
    system->evaporation_rate = evaporation_rate;
    // All the arrays and matrix
    system->cities = initialize_cities_list(system->n_cities);
    system->cities_distances = initialize_distances(n_cities);
    system->pheromones = initialize_pheromones(n_cities);
    system->list_tabu_list = initialize_paths(n_ants, n_cities);

    // Solutions
    // Best solution path is not initialized, because the pointer is set after the first cycle
    system->best_solution_cost = INT_MAX;
    system->best_solution = (int *)malloc(sizeof(int) * n_cities);

    return system;
}

// Increments pheromones of the best solution by 1
void increment_pheromones(ant_system *system)
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
    int *visited;

    a = initArray(1);

    visited = visited_cities(s, ant_number);
    
    for (int i = 0; i < s->n_cities; i++)
    {
        if (is_value_in_array(visited, s->n_cities, i) == 0)
        {
            insertArray(a, i);
        }
    }

    return a;
}

Array *available_next_cities(Array *a, int current)
{
    Array *array;

    array = initArray(1);

    for (int i = 0; i < a->used; i++)
    {
        if (a->array[i] != current)
        {
            insertArray(array, a->array[i]);
        }
    }

    return a;
}

int current_city(ant_system *s, int ant_number, int iter)
{
    return s->list_tabu_list[ant_number][iter];
}

// Returns the number of the next city to move
int next_city(ant_system *as, int ant_number, int iter)
{
    int current, next, next_city, available;
    double *probabilities;
    Array *unvisited_cities, *available_cities;

    // roulette wheel selection:
    int distance;
    double inverse_distance, numerator, pheromone, denominator = 0;

    current = current_city(as, ant_number, iter);
    unvisited_cities = not_visited_cities(as, ant_number);
    available_cities = available_next_cities(unvisited_cities, current);

    //print_int_array(unvisited_cities->array, unvisited_cities->size);
    //print_int_array(available_cities->array, available_cities->size);

    probabilities = (double *)malloc(sizeof(double) *
                                     available_cities->size);
    for (int i = 0; i < available_cities->size; i++)
    {
        //next = available_cities->array[i];
        //distance = as->cities_distances[current][next];
        //inverse_distance = 1.0f / distance;
        available = available_cities->array[i];
        pheromone = as->pheromones[current][available];
        numerator = pow(pheromone, as->alpha);
                    //*pow(inverse_distance, as->beta);
        denominator += numerator;
        probabilities[i] = numerator;
    }
    //printf("denominator: %f\n", denominator);
    for (int i = 0; i < available_cities->size; i++)
    {
        probabilities[i] /= denominator;
    }

    int idx = random_from_discrete_distribution(probabilities,
                                                available_cities->size);
    if (idx == -1)
    {
        printf("IT BROKE DURING NEXT CITY SELECTION");
        exit(64);
    }

    next_city = available_cities->array[idx];

    freeArray(available_cities);
    free(probabilities);

    return next_city;
}
// iter -> The movement number.
// i.e The first time the ant moves (iter = 1),
// will set tabu_list[1] (0 is already defined on initialization) when moves
// from the initial city to the first target
void move_to_city(ant_system *s, int ant, int iter, int city)
{
    s->list_tabu_list[ant][iter] = city;
}

// Returns the sum for the path, based on the distances defined in the system
int path_cost(ant_system *s, int path_idx)
{
    int *path, current, next, sum = 0;

    path = s->list_tabu_list[path_idx];

    for (int i = 0; i < (s->n_cities - 1); i++)
    {
        current = path[i];
        next = path[i + 1];
        sum += s->cities_distances[current][next];
    }
    return sum;
}

int best_path_idx(ant_system *s)
{

    int best_aux_path_idx, best_aux_cost = INT_MAX, aux_cost = 0;

    for (int i = 0; i < s->n_ants; i++)
    {

        aux_cost = path_cost(s, i);
        if (aux_cost < best_aux_cost)
        {
            best_aux_cost = aux_cost;
            best_aux_path_idx = i;
        }
    }

    return best_aux_path_idx;
}

void best_solution(ant_system *s)
{
    int best_idx, aux_best_cost;

    best_idx = best_path_idx(s);
    aux_best_cost = path_cost(s, best_idx);
    // Will update the best solution if is shorter than the already saved
    // (The first iteration is INT_MAX)
    if (aux_best_cost < s->best_solution_cost)
    {
        // Note: Im not really sure why the size for the copy is this, but it works
        memcpy(s->best_solution,
               s->list_tabu_list[best_idx],
               sizeof(s->best_solution[0]) * s->n_cities);
        s->best_solution_cost = aux_best_cost;
    }
}

void update_pheromones(ant_system *s)
{
    int current, next;
    // Note: Can be optimized merging both loops into 1
    double evaporation = 1.0 - s->evaporation_rate;
    double reinforcement = 0.5; //3.0 s->best_solution_cost; // 1.0 cast the division to double TODO

    for (int i = 0; i < s->n_cities; i++)
    {
        for (int j = 0; j < s->n_cities; j++)
        {
            // Evaporation
            s->pheromones[i][j] *= evaporation;
        }
    }

    // Reinforcement pheromones only on the best solution
    // edges
    for (int i = 0; i < (s->n_cities - 1); i++)
    {
        current = s->best_solution[i];
        next = s->best_solution[i + 1];
        // Reinforcement
        s->pheromones[current][next] += reinforcement;
    }
}

void reset_tabu_list(ant_system *s)
{
    free_matrix(s->list_tabu_list, s->n_ants);
    s->list_tabu_list = initialize_paths(s->n_ants, s->n_cities);
}