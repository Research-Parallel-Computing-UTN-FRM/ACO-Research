#include <curand.h>
#include <cuda.h>
#include <curand_kernel.h>
#include <stdio.h>

#define THREADS 10
#define BLOCKS 10
#define N 10
__global__ void kernel_init(curandState *state)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    curand_init(1337, idx, 0, &state[idx]);
}

__global__ void kernel_make_rand(curandState *state, float *values)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < N)
        values[idx] = curand_uniform(&state[idx]);
}
int main()
{
    float *h_values;
    h_values = (float *)malloc(sizeof(float) * N);

    curandState *d_state;
    float *d_values;

    cudaMalloc(&d_values, sizeof(float) * N);
    cudaMalloc(&d_state, THREADS * BLOCKS);
    kernel_init<<<BLOCKS, THREADS>>>(d_state);

    kernel_make_rand<<<BLOCKS, THREADS>>>(d_state, d_values);

    cudaDeviceSynchronize();

    cudaMemcpy(h_values, d_values, sizeof(float) * N, cudaMemcpyKind::cudaMemcpyDeviceToHost);

    printf("a: [");
    for (int i = 0; i < N - 1; i++)
    {
        printf("%f", h_values[i]);
        if (i < N - 2)
            printf(", ");
    }
    printf("]\n");

    free(h_values);

    cudaFree(d_values);
    cudaFree(d_state);
    cudaDeviceReset();

    return 0;
}