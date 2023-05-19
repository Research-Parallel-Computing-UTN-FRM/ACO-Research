#include <stdio.h>

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

// void push_item(int *stack)
// {
//     stack
// }