#include "array.h"
struct ant_system
{
    float alpha;
    float beta;
    int n_ants;
    int n_cities;
    float evaporation_rate;
    int *cities;            // {x: x belongs Z0 & x < n_cities} -> [0, 1, ..., n_cities]
                            // Useful when you have to check if a cities is already visited
    int **cities_distances; // n_cities x n_cities matrix
    int **pheromones;       // n_cities x n_cities matrix
    int **list_tabu_list;   // n_ants x n_cities matrix
                            // List of tabu list. Each array inside the
                            // matrix has the path followed by one ant
};

typedef struct ant_system ant_system;

ant_system *new_system(int n_cities, int n_ants, float alpha, float beta, float evaporation_rate);

void update_pheromone(ant_system *system);

double city_probability(ant_system *s, int current, int city);

int *visited_cities(ant_system *s, int ant_number);

Array *not_visited_cities(ant_system *s, int ant_number);

int next_city(ant_system *s, int ant_number, int iter);
void move_to_city(ant_system *s, int ant, int iter, int city);