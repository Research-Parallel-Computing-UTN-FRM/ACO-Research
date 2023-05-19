#include <stdlib.h>
#include <stdio.h>
#include "initialize.h"
#include "system.h"
#include "util.h"

ant_system *malloc_system(int n_cities)
{
    // Malloc the  struct
    ant_system *system = (struct ant_system *)malloc(sizeof(struct ant_system));
    // Malloc the array
    system->visited_cities = (int *)malloc(sizeof(int) * n_cities);

    // Malloc matrix
    system->cities_distances = (int **)malloc(sizeof(int *) * n_cities);
    system->pheromones = (int **)malloc(sizeof(int *) * n_cities);
    for (int i = 0; i < n_cities; i++)
    {
        system->cities_distances[i] = (int *)malloc(sizeof(int) * n_cities);
        system->pheromones[i] = (int *)malloc(sizeof(int) * n_cities);
    }

    return system;
}

ant_system *new_system(int n_cities, int n_ants, float alpha, float beta)
{
    ant_system *system;

    system = malloc_system(n_cities);
    if (system == NULL)
        return NULL;

    system->n_cities = n_cities;
    system->n_ants = n_ants;
    system->alpha = alpha;
    system->cities_distances = initialize_distances(n_cities);
    system->pheromones = initialize_pheromones(n_cities);

    return system;
}

void update_pheromone(ant_system *system)
{
    for (int i = 0; i < system->n_cities; i++)
    {
        printf("RAN");
        for (int j; j = system->n_cities; j++)
        {
            // printf("%d", system->pheromones[i][j]);
            system->pheromones[i][j]++;
        }
    }
}

void print_system(ant_system *system)
{
    printf("--------------------------- \n");
    printf("System: \n\n");
    printf("Number of cities: %d \n\n", system->n_cities);
    printf("Number of ants: %d \n\n", system->n_ants);
    printf("Heuristic params: alpha %f  beta %f \n\n", system->alpha, system->beta);

    printf("Pheromones: \n");
    print_int_matrix(system->pheromones, system->n_cities, system->n_cities);
    printf("\n");

    printf("Distances: \n");
    print_int_matrix(system->cities_distances, system->n_cities, system->n_cities);
    printf("\n");

    printf("--------------------------- \n");
}