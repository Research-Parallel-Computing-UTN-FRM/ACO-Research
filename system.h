struct ant_system
{
    float alpha;
    float beta;
    int n_ants;
    int n_cities;
    int **cities_distances; // n_cities x n_cities matrix
    int **pheromones;       // n_cities x n_cities matrix
    int *visited_cities;    // This should work as a stack
};

typedef struct ant_system ant_system;

ant_system *new_system(int n_cities, int n_ants, float alpha, float beta);

void update_pheromone(ant_system *system);

void print_system(ant_system *system);