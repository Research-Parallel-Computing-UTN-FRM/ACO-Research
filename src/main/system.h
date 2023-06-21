typedef struct AntSystem
{
    int total_cycles;
    float alpha;
    float beta;
    int total_ants;
    int total_cities;
    int starting_city;
    float evaporation_rate;
    int best_solution_cost;
    int *best_solution;
    int *cities;
    int **cities_distances;
    double **pheromones;
    int **tabu_list;
} AntSystem;

AntSystem *create_ant_system(
    int total_cities,
    int total_ants,
    int starting_city,
    float alpha,
    float beta,
    float evaporation_rate,
    int total_cycles);

void free_ant_system(AntSystem *s);

void initialize_random_cities_distances(AntSystem *ant_system, int max_distance);

int *ant_system_cycle(AntSystem *ant_system);