int **initialize_distances(int size)
{
    int **distances = malloc(sizeof(int *) * size);
    for (int i = 0; i < size; i++)
    {
        distances[i] = malloc(sizeof(int) * size);
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            // Generate a symmetric matrix
            distances[i][j] = rand() % 500;
            distances[j][i] = distances[i][j];
        }
    }
    for (int i = 0; i < size; i++)
    {
        distances[i][i] = 0;
    }
    return distances;
}

int **intialize_pheromones(int size)
{
    int **pheromones = malloc(sizeof(int *) * size);
    for (int i = 0; i < size; i++)
    {
        pheromones[i] = malloc(sizeof(int) * size);
    }
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            pheromones[i][j] = 1;
        }
    }
    return pheromones;
}