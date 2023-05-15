#include <stdlib.h>
struct ant_system
{
    float alpha;
    float beta;
    int n_ants;
    int n_cities;
    int **cities_distances; // n_cities x n_cities matrix
    int **pheromones;       // n_cities x n_cities matrix
    int *visited_cities;
};
typedef ant_system ant_system;

ant_system *malloc_system(int n_cities)
{
    // Malloc the  struct
    struct ant_system *system = (struct ant_system *)malloc(sizeof(struct ant_system));
    // Malloc the array
    system->visited_cities = (int *)malloc(sizeof(int) * n_cities);

    // Malloc matrixs
    system->cities_distances = (int **)malloc(sizeof(int *) * n_cities);
    system->pheromones = (int **)malloc(sizeof(int *) * n_cities);
    for (int i = 0; i < n_cities; i++)
    {
        system->cities_distances[i] = (int *)malloc(sizeof(int) * n_cities);
        system->pheromones[i] = (int *)malloc(sizeof(int) * n_cities);
    }

    return system;
}

ant_system *new_system(int n_cities, float alpha, float beta)
{
    ant_system *ant_system;

    ant_system = malloc_system(n_cities);
    if (ant_system == NULL)
        return NULL;

    return ant_system;
}