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

    a = initArray(1);
    // printf("Ant number: %d \n", ant_number);
    // print_int_array(s->list_tabu_list[ant_number], s->n_cities);
    for (int i = 0; i < s->n_cities; i++)
    {
        if (is_value_in_array(visited_cities(s, ant_number), s->n_cities, i) == 0)
        {
            insertArray(a, i);
        }
    }
    // printf("array size: %ld \n", a->size);
    // printf("Not visited citites: ");
    // print_int_array(a->array, a->size);
    return a;
}

int current_city(ant_system *s, int ant_number, int iter)
{
    return s->list_tabu_list[ant_number][iter];
}

// Returns the number of the next city to move
int next_city(ant_system *as, int ant_number, int iter)
{
    int current, next_idx, next;
    double *probabilities;
    Array *unvisited_cities;
    // roulette wheel selection:
    int distance, pheromone;
    double inverse_distance, numerator, denominator = 0;

    current = current_city(as, ant_number, iter);
    unvisited_cities = not_visited_cities(as, ant_number);
    probabilities = (double *)malloc(sizeof(int) * unvisited_cities->size);

    for (int i = 0; i < unvisited_cities->size; i++)
    {
        distance = as->cities_distances[current][unvisited_cities->array[i]];
        inverse_distance = 1.0f / distance;
        pheromone = as->pheromones[current][unvisited_cities->array[i]];
        numerator = pow(pheromone, as->alpha) * pow(inverse_distance, as->beta);
        denominator += numerator;
        probabilities[i] = numerator;
    }
    for (int i = 0; i < unvisited_cities->size; i++)
    {
        probabilities[i] /= denominator;
    }
    // printf("\n\nProbabilities: \n");
    // for (int i = 0; i < unvisited_cities->size; i++)
    // {
    //     printf("City:%d P:%f\n", unvisited_cities->array[i], probabilities[i]);
    // }
    int idx = random_from_discrete_distribution(probabilities, unvisited_cities->size);
    // printf("The selected city is %d\n", unvisited_cities->array[idx]);
    return unvisited_cities->array[idx];
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
        memcpy(s->best_solution, s->list_tabu_list[best_idx], sizeof(s->best_solution[0]) * s->n_cities);
        s->best_solution_cost = aux_best_cost;
    }
}

void update_pheromones(ant_system *s)
{
    double evaporation = 1.0 - s->evaporation_rate;
    double reinforcement = 1.0 / s->best_solution_cost; // 1.0 cast the division to double
    printf("\nREINFORCEMENT: %f \n", reinforcement);
    for (int i = 0; i < s->n_cities; i++)
    {
        for (int j = 0; j < s->n_cities; j++)
        {
            // Evaporation
            s->pheromones[i][j] *= evaporation;
            // Reinforcement
            s->pheromones[i][j] += reinforcement;
        }
    }
}

void reset_tabu_list(ant_system *s)
{
    free_matrix(s->list_tabu_list, s->n_ants);
    s->list_tabu_list = initialize_paths(s->n_ants, s->n_cities);
}