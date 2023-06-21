#include <stdbool.h>

void print_int_matrix(int **matrix, int rows, int columns);

void print_double_matrix(double **matrix, int rows, int columns);

void print_int_vector(int *vector, int columns);

void print_double_vector(double *vector, int columns);

bool int_in_vector(int *vector, int columns, int value);

int sum_int_vector(int *vector, int columns);