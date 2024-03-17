/*
 *Programmer(s):Nabeeha , Arwa and Alisbha
 *Desc: MPI version of matrix vector multiplication using Tiling.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define TILE_DIM 2
#define MAX_VALUE 100

void matrixVectorMultiplication(int* matrix, int* vector, int* result, int n, int rank, int num_procs) {
    int i, j, ii, jj;
    int cij;

    for (ii = rank; ii < n; ii += num_procs) {
        for (jj = 0; jj < n; jj += TILE_DIM) {
            for (i = ii; i < ii + TILE_DIM && i < n; ++i) {
                cij = 0;
                for (j = jj; j < jj + TILE_DIM && j < n; ++j)
                    cij += matrix[i * n + j] * vector[j];
                result[i] += cij;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    int rank, num_procs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (argc != 2) {
        if (rank == 0)
            printf("Usage: %s <matrix_size>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    int N = atoi(argv[1]);

    srand(time(NULL)); // Seed the random number generator

    int* matrix = (int*)malloc(N * N * sizeof(int));
    int* vector = (int*)malloc(N * sizeof(int));
    int* result = (int*)calloc(N, sizeof(int));

    // Generate random matrix and vector elements
    if (rank == 0) {
        printf("MPI Tile Implementation");
        printf("Random Matrix:\n");
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i * N + j] = rand() % (MAX_VALUE + 1); // Generate random integer between 0 and MAX_VALUE
            if (rank == 0) {
                printf("%d ", matrix[i * N + j]);
            }
        }
        if (rank == 0) {
            printf("\n");
        }
        vector[i] = rand() % (MAX_VALUE + 1); // Generate random integer between 0 and MAX_VALUE
    }

    if (rank == 0) {
        printf("Random Vector:\n");
        for (int i = 0; i < N; i++) {
            printf("%d ", vector[i]);
        }
        printf("\n");
    }

    matrixVectorMultiplication(matrix, vector, result, N, rank, num_procs);

    // Reduce the results from all processes to process 0
    MPI_Reduce(rank == 0 ? MPI_IN_PLACE : result, result, N, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Resultant vector after multiplication:\n");
        for (int i = 0; i < N; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
    }

    free(matrix);
    free(vector);
    free(result);

    MPI_Finalize();
    return 0;
}
