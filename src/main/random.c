#include <stdlib.h>
#include <stdio.h>

int random_integer(int max)
{
    // Returns a random integer in [0, max]
    return rand() % (max + 1);
}

double random_zero_to_one()
{
    // Returns a random number in [0, 1]
    return (double)rand() / (double)RAND_MAX;
}

int discrete_random_variable(double *probabilities, int size)
{
    /* Returns the index of the element randomly selected */
    double rand = random_zero_to_one();
    double cumulative_probability = 0.0f;

    for (int i = 0; i < size; i++)
    {
        cumulative_probability += probabilities[i];
        if (rand <= cumulative_probability)
        {
            return i;
        }
    }
    printf("Error: discrete_random_variable\n");
    printf("cumulative_probability: %f\n", cumulative_probability);
    return -1;
}
