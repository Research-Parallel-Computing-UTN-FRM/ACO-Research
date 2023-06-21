#include <stdlib.h>
#include "random.h"
#include "initialize.h"

int **initialize_distances(int size, int max_distance)
{
    int **distances = (int **)malloc(sizeof(int *) * size);

    for (int i = 0; i < size; i++)
    {
        distances[i] = malloc(sizeof(int) * size);
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            distances[i][j] = random_integer(max_distance);
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

int **initialize_tabu_list(int rows, int columns)
{
    int **tabu_list = (int **)malloc(sizeof(int *) * rows);
    for (int i = 0; i < rows; i++)
    {
        tabu_list[i] = malloc(sizeof(int) * columns);
    }
    for (int i = 0; i < rows; i++)
    {
        int r = random_integer(columns - 1);
        tabu_list[i][0] = r;
        for (int j = 1; j < columns; j++)
        {
            tabu_list[i][j] = -1;
        }
    }
    return tabu_list;
}

int *initialize_cities(int total_cities)
{
    int *cities = (int *)malloc(sizeof(int) * total_cities);
    for (int i = 0; i < total_cities; i++)
    {
        cities[i] = i;
    }
    return cities;
}