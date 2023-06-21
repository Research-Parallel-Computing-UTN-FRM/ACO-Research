#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "system.h"
#include "utils.h"
#include "random.h"

AntSystem *malloc_ant_system(int total_cities, int total_ants)
{
    AntSystem *ant_system = (struct AntSystem *)malloc(sizeof(struct AntSystem));
    ant_system->cities = (int *)malloc(sizeof(int) * total_cities);
    ant_system->tabu_list = (int **)malloc(sizeof(int *) * total_ants);
    ant_system->cities_distances = (int **)malloc(sizeof(int *) * total_cities);
    ant_system->pheromones = (double **)malloc(sizeof(double *) * total_cities);

    int i;
    for (i = 0; i < total_ants; i++)
    {
        ant_system->tabu_list[i] = (int *)malloc(sizeof(int) * total_cities);
    }

    for (i = 0; i < total_cities; i++)
    {
        ant_system->cities_distances[i] = (int *)malloc(sizeof(int) * total_cities);
        ant_system->pheromones[i] = (double *)malloc(sizeof(double) * total_cities);
    }

    return ant_system;
}

void initialize_random_cities_distances(AntSystem *ant_system, int max_distance)
{
    int size = ant_system->total_cities;
    int random_distance;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            random_distance = random_integer(max_distance);
            if (random_distance == 0)
                random_distance = 1;
            ant_system->cities_distances[i][j] = random_distance;
            ant_system->cities_distances[j][i] = random_distance;
        }
    }
    for (int i = 0; i < size; i++)
    {
        ant_system->cities_distances[i][i] = INT_MAX;
    }
}

static void initialize_pheromones(AntSystem *ant_system)
{
    int size = ant_system->total_cities;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            ant_system->pheromones[i][j] = 1.0f;
        }
    }
}

static void initialize_tabu_list(AntSystem *ant_system)
{
    for (int ant = 0; ant < ant_system->total_ants; ant++)
    {
        // First city
        if (ant_system->starting_city == -1)
            ant_system->tabu_list[ant][0] = random_integer(ant_system->total_cities - 1);
        else
            ant_system->tabu_list[ant][0] = ant_system->starting_city;
        // Other cities
        for (int city = 1; city < ant_system->total_cities; city++)
            ant_system->tabu_list[ant][city] = -1;
    }
}

static void initialize_cities(AntSystem *ant_system)
{
    for (int i = 0; i < ant_system->total_cities; i++)
        ant_system->cities[i] = i;
}

void free_ant_system(AntSystem *ant_system)
{
    int i;
    for (i = 0; i < ant_system->total_cities; i++)
    {
        free(ant_system->cities_distances[i]);
        free(ant_system->pheromones[i]);
    }
    free(ant_system->cities_distances);
    free(ant_system->pheromones);

    for (i = 0; i < ant_system->total_ants; i++)
    {
        free(ant_system->tabu_list[i]);
    }
    free(ant_system->tabu_list);
    free(ant_system->cities);

    free(ant_system);
}

AntSystem *create_ant_system(
    int total_cities,
    int total_ants,
    int starting_city,
    float alpha,
    float beta,
    float evaporation_rate,
    int total_cycles)
{
    AntSystem *ant_system;

    ant_system = malloc_ant_system(total_cities, total_ants);
    if (ant_system == NULL)
        return NULL;

    ant_system->total_cycles = total_cycles;
    ant_system->total_cities = total_cities;
    ant_system->total_ants = total_ants;
    ant_system->starting_city = starting_city;
    ant_system->alpha = alpha;
    ant_system->beta = beta;
    ant_system->evaporation_rate = evaporation_rate;
    ant_system->best_solution_cost = INT_MAX;
    ant_system->best_solution = (int *)malloc(sizeof(int) * total_cities);

    initialize_cities(ant_system);
    initialize_pheromones(ant_system);
    return ant_system;
}

static void next_city(AntSystem *ant_system, int ant_number, int current_city_index)
{
    // Find unvisited cities
    int total_unvisited_cities = ant_system->total_cities - current_city_index - 1;
    int *unvisited_cities = (int *)malloc(sizeof(int) * total_unvisited_cities);
    int i = 0;

    for (int city = 0; city < ant_system->total_cities; city++)
    {
        if (!int_in_vector(ant_system->tabu_list[ant_number], ant_system->total_cities, city))
        {
            unvisited_cities[i] = city;
            i++;
        }
    }
    // Compute probabilities
    int unvisited_city;
    double inverse_distance, pheromone, probability;
    int current_city = ant_system->tabu_list[ant_number][current_city_index];
    double *probabilities = (double *)malloc(sizeof(double) * total_unvisited_cities);

    double probability_sum = 0.0f;
    for (i = 0; i < total_unvisited_cities; i++)
    {
        unvisited_city = unvisited_cities[i];
        inverse_distance = (float)1 / ant_system->cities_distances[current_city][unvisited_city];
        pheromone = ant_system->pheromones[current_city][unvisited_city];
        probability = pow(pheromone, ant_system->alpha) * pow(inverse_distance, ant_system->beta);
        probability_sum += probability;
        probabilities[i] = probability;
    }

    for (i = 0; i < total_unvisited_cities; i++)
    {
        probabilities[i] /= probability_sum;
    }

    // Find next city
    int next_city_index = discrete_random_variable(probabilities, total_unvisited_cities);
    ant_system->tabu_list[ant_number][current_city_index + 1] = unvisited_cities[next_city_index];
}

static int solution_cost(AntSystem *ant_system, int *solution)
{
    /*
    TODO: FIX SOLUTION COST

distances:
2147483647 51 278 445
51 2147483647 266 85
278 266 2147483647 332
445 85 332 2147483647

best solution found:
1 0 3 2

best solution cost: 414
    */
    int total_cost = 0;
    int current_city = solution[0];
    int next_city_;
    for (int city_index = 1; city_index < ant_system->total_cities; city_index++)
    {
        next_city_ = solution[city_index];
        total_cost += ant_system->cities_distances[current_city][next_city_];
        current_city = next_city_;
    }
    return total_cost;
}

static int *get_best_cycle_solution(AntSystem *ant_system)
{
    int current_solution_cost;
    int best_solution_index = 0;
    int best_solution_cost = INT_MAX;
    for (int solution_index = 0; solution_index < ant_system->total_ants; solution_index++)
    {
        current_solution_cost = solution_cost(
            ant_system,
            ant_system->tabu_list[solution_index]);
        if (current_solution_cost < best_solution_cost)
        {
            best_solution_cost = current_solution_cost;
            best_solution_index = solution_index;
        }
    }
    return ant_system->tabu_list[best_solution_index];
}

static void update_pheromones(AntSystem *ant_system, int *best_cycle_solution, int best_cycle_solution_cost)
{
    // Evaporation phase
    for (int i = 0; i < ant_system->total_cities; i++)
    {
        for (int j = 0; j < ant_system->total_cities; j++)
        {
            ant_system->pheromones[i][j] *= (1 - ant_system->evaporation_rate);
        }
    }

    // Reinforcement phase
    int static current_city, next_city;
    for (int i = 0; i < ant_system->total_cities - 1; i++)
    {
        current_city = best_cycle_solution[i];
        next_city = best_cycle_solution[i + 1];
        ant_system->pheromones[current_city][next_city] += 1 / best_cycle_solution_cost;
    }
}

int *ant_system_cycle(AntSystem *ant_system)
{
    // Reset tabu list
    initialize_tabu_list(ant_system);

    // Run cycle
    for (int ant = 0; ant < ant_system->total_ants; ant++)
    {
        for (int city = 0; city < ant_system->total_cities - 1; city++)
        {
            next_city(ant_system, ant, city);
        }
    }
    // Update pheromones
    int *best_cycle_solution = get_best_cycle_solution(ant_system);
    int best_cycle_solution_cost = solution_cost(ant_system, best_cycle_solution);
    update_pheromones(ant_system, best_cycle_solution, best_cycle_solution_cost);
    // Update best solution
    if (best_cycle_solution_cost < ant_system->best_solution_cost)
    {
        ant_system->best_solution_cost = best_cycle_solution_cost;
        ant_system->best_solution = best_cycle_solution;
    }
    return best_cycle_solution;
}