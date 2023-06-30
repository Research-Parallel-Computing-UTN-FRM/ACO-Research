#include <stdio.h>

__global__ void sum(int *a, int *b, int *c, int N)
{
    int idx = threadIdx.x;
    if (idx < N)
        c[idx] = a[idx] + b[idx];
}

int main(int argc, char *argv[])
{
    int N;
    int *h_a, *h_b, *h_c;
    int *d_a, *d_b, *d_c;

    N = atoi(argv[1]);

    h_a = (int *)malloc(sizeof(int) * N);
    h_b = (int *)malloc(sizeof(int) * N);
    h_c = (int *)malloc(sizeof(int) * N);

    for (int i = 0; i < N - 1; i++)
    {
        h_a[i] = i * 2;
        h_b[i] = i * 3;
    }

    // This pointer to pointer is part of the definition. Dunno why is like that
    cudaMalloc(&d_a, sizeof(int) * N);
    cudaMalloc(&d_b, sizeof(int) * N);
    cudaMalloc(&d_c, sizeof(int) * N);

    cudaMemcpy(d_a, h_a, sizeof(int) * N, cudaMemcpyKind::cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, sizeof(int) * N, cudaMemcpyKind::cudaMemcpyHostToDevice);

    printf("HELLO FROM cpu %d\n", N);

    sum<<<1, N + 2>>>(d_a, d_b, d_c, N);

    cudaDeviceSynchronize();

    cudaMemcpy(h_c, d_c, sizeof(int) * N, cudaMemcpyKind::cudaMemcpyDeviceToHost);

    printf("a: [");
    for (int i = 0; i < N - 1; i++)
    {
        printf("%d ", h_a[i]);
    }
    printf("]\n");

    printf("b: [");
    for (int i = 0; i < N - 1; i++)
    {
        printf("%d ", h_b[i]);
    }
    printf("]\n");

    printf("c: [");
    for (int i = 0; i < N - 1; i++)
    {
        printf("%d ", h_c[i]);
    }
    printf("]\n");

    free(h_a);
    free(h_b);
    free(h_c);

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    cudaDeviceReset();
    return 0;
}