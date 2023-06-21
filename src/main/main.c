#include <stdlib.h>
#include <stdio.h>
#include "system.h"
#include "utils.h"

void print_ant_system(const AntSystem *ant_system)
{
    printf("total_cycles: %d\n", ant_system->total_cycles);
    printf("alpha: %f\n", ant_system->alpha);
    printf("beta: %f\n", ant_system->beta);
    printf("total_ants: %d\n", ant_system->total_ants);
    printf("total_cities: %d\n", ant_system->total_cities);
    printf("evaporation_rate: %f\n", ant_system->evaporation_rate);
    printf("best_solution_cost: %d\n", ant_system->best_solution_cost);
    printf("best_solution:\n");
    print_int_vector(ant_system->best_solution, ant_system->total_cities);
    printf("cities:\n");
    print_int_vector(ant_system->cities, ant_system->total_cities);
    printf("cities_distances:\n");
    print_int_matrix(ant_system->cities_distances, ant_system->total_cities, ant_system->total_cities);
    printf("pheromones:\n");
    print_double_matrix(ant_system->pheromones, ant_system->total_cities, ant_system->total_cities);
    printf("tabu_list:\n");
    print_int_matrix(ant_system->tabu_list, ant_system->total_ants, ant_system->total_cities);
}

int main()
{
    // Ant system configuration
    int total_cycles = 100;
    float alpha = 1;
    float beta = 1;
    int total_ants = 400;
    int total_cities = 10;
    int starting_city = -1; // random
    float evaporation_rate = 0.5;
    int max_distance = 500;

    // Random seed
    const int SEED = 42;
    srand(SEED);

    // Create ant system
    AntSystem *ant_system;
    ant_system = create_ant_system(
        total_cities,
        total_ants,
        starting_city,
        alpha,
        beta,
        evaporation_rate,
        total_cycles);
    initialize_random_cities_distances(ant_system, max_distance);

    // Algorithm execution
    printf("distances:\n");
    print_int_matrix(ant_system->cities_distances, ant_system->total_cities, ant_system->total_cities);
    for (int cycle = 0; cycle < ant_system->total_cycles; cycle++)
    {
        ant_system_cycle(ant_system);
    }
    printf("\nbest solution found:\n");
    print_int_vector(ant_system->best_solution, ant_system->total_cities);
    printf("\nbest solution cost: %d\n", ant_system->best_solution_cost);
    return 0;
}
