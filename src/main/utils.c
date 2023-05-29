#include <stdio.h>
#include "system.h"
#include <stdlib.h>
#include <time.h>

void print_int_matrix(int **matrix, int rows, int columns)
{

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void print_int_array(int *array, int columns)
{

    for (int j = 0; j < columns; j++)
    {
        printf("%d ", array[j]);
    }
    printf("\n");
}

void print_double_array(double *array, int columns)
{

    for (int j = 0; j < columns; j++)
    {
        printf("%f ", array[j]);
    }
    printf("\n");
}

void print_system(ant_system *system)
{
    printf("--------------------------- \n");
    printf("System: \n\n");
    printf("Number of cities: %d \n\n", system->n_cities);
    printf("Number of ants: %d \n\n", system->n_ants);
    printf("Heuristic params: alpha %f  beta %f \n\n", system->alpha, system->beta);
    printf("Evaporation rate: %f \n\n", system->evaporation_rate);

    // Printing the list of tabu list makes the log unredeable
    // Uncomment if its necessary for debugging
    // printf("Visited cities: ");
    // print_int_matrix(system->list_tabu_list, system->n_ants, system->n_cities);

    printf("Pheromones: \n");
    print_int_matrix(system->pheromones, system->n_cities, system->n_cities);
    printf("\n");

    printf("Distances: \n");
    print_int_matrix(system->cities_distances, system->n_cities, system->n_cities);
    printf("\n");

    printf("--------------------------- \n");
}

// It assumes that you already called srand before using it
int random_int(int max)
{
    return rand() % (max + 1);
}

// Returns a random number between [0;1]
// Assumes that you already called stand before using it
double random_zero_one()
{
    return (double)rand() / (double)RAND_MAX;
}

// Will check if a value exists in a SORTED array
// The fact that the array is ordered is important
// because it does some checks to improve speed on large arrays.
// Return 1 if exists, 0 otherwise
int is_value_in_array(int *array, int arr_len, int v)
{
    for (int i = 0; i < arr_len; i++)
    {
        if (array[i] == v)
            return 1;
        // if (sorted == NULL && array[i] < v)
        //     return 0;
    }
    return 0;
}

int random_from_discrete_distribution(double *probabilities, int size)
{
    /* Returns the index of the element randomly selected*/
    double rand;
    double cumulative_probability = 0.0;

    rand = random_zero_one();
    printf("Random number: %f \n", rand);
    for (int i = 0; i < size; i++)
    {
        cumulative_probability += probabilities[i];
        if (rand <= cumulative_probability)
            return i;
    }
    return -1;
}