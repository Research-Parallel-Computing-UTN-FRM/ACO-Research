#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
void print_double_matrix(double **matrix, int rows, int columns)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            printf("%f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void print_int_vector(int *vector, int columns)
{
    for (int i = 0; i < columns; i++)
    {
        printf("%d ", vector[i]);
    }
    printf("\n");
}

void print_double_vector(double *vector, int columns)
{

    for (int i = 0; i < columns; i++)
    {
        printf("%f ", vector[i]);
    }
    printf("\n");
}

bool int_in_vector(int *vector, int columns, int value)
{
    for (int i = 0; i < columns; i++)
    {
        if (vector[i] == value)
        {
            return true;
        }
    }
    return false;
}

int sum_int_vector(int *vector, int columns)
{
    int sum = 0;
    for (int i = 0; i < columns; i++)
    {
        sum += vector[i];
    }
    return sum;
}