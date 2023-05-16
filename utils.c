#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

float cost(int **distances, int distances_size, int *solution, int solution_size, bool round_trip)
{
    float cost = 0;
    for (int i = 0; i < solution_size - 1; i++)
    {
        cost += distances[solution[i]][solution[i + 1]];
    }
    if (round_trip)
    {
        cost += distances[solution[solution_size - 1]][solution[0]];
    }
    return cost;
}