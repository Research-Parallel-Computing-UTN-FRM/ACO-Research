#include "initialize.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "system.h"
#include "utils.h"

int **initialize_distances(int size)
{
    int **distances = (int **)malloc(sizeof(int *) * size);
    for (int i = 0; i < size; i++)
    {
        distances[i] = malloc(sizeof(int) * size);
    }

    srand(time(NULL)); // Initialization, should only be called once.
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            // Generate a symmetric matrix
            distances[i][j] = random_int(500);
            distances[j][i] = distances[i][j];
        }
    }
    for (int i = 0; i < size; i++)
    {
        distances[i][i] = 0;
    }
    return distances;
}

double **initialize_pheromones(int size)
{
    double **pheromones = (double **)malloc(sizeof(double *) * size);
    for (int i = 0; i < size; i++)
    {
        pheromones[i] = malloc(sizeof(double) * size);
    }
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            pheromones[i][j] = 1.0f;
        }
    }
    return pheromones;
}

int **initialize_paths(int rows, int columns)
{
    int **tabus = (int **)malloc(sizeof(int *) * rows);
    for (int i = 0; i < rows; i++)
    {
        tabus[i] = malloc(sizeof(int) * columns);
    }

    // Set an initial node for every ant
    srand(time(NULL)); // Initialization, should only be called once.
    for (int i = 0; i < rows; i++)
    {
        int r = random_int(columns - 1);
        tabus[i][0] = r;
    }

    return tabus;
}

int *initialize_cities_list(int n_cities)
{

    int *cities = (int *)malloc(sizeof(int) * n_cities);
    for (int i = 0; i < n_cities; i++)
    {
        cities[i] = i;
    }
    return cities;
}