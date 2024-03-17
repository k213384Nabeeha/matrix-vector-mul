/*
 *Programmer(s):Nabeeha, Arrwa and Alisbha
 *Desc: MPI version of matrix vector multiplication Naive.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void fill_matrix(int** matrix, int rows, int cols) {
    // Fill the matrix with random values
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 10; // Random values between 0 and 9
        }
    }
}

void fill_vector(int* vector, int size) {
    // Fill the vector with random values
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 10; // Random values between 0 and 9
    }
}

void print_matrix(int** matrix, int rows, int cols) {
    printf("Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void print_vector(int* vector, int size) {
    printf("Vector:\n");
    for (int i = 0; i < size; i++) {
        printf("%d\n", vector[i]);
    }
}

void print_result(int* result, int size) {
    printf("Result:\n");
    for (int i = 0; i < size; i++) {
        printf("%d\n", result[i]);
    }
}

void matrix_vector_multiply(int** matrix, int* vector, int* result, int rows, int cols) {
    // Perform matrix-vector multiplication using MPI
    for (int i = 0; i < rows; i++) {
        result[i] = 0;
        for (int j = 0; j < cols; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 3) {
        if (rank == 0) {
            printf("Usage: %s <matrix size> <vector size>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    int matrix_size = atoi(argv[1]);
    int vector_size = atoi(argv[2]);

    // Allocate memory for matrix, vector, and result
    int** matrix = (int**)malloc(matrix_size * sizeof(int*));
    for (int i = 0; i < matrix_size; i++) {
        matrix[i] = (int*)malloc(vector_size * sizeof(int));
    }

    int* vector = (int*)malloc(vector_size * sizeof(int));
    int* result = (int*)malloc(matrix_size * sizeof(int));

    // Fill the matrix and vector with random values on root process
    if (rank == 0) {
        printf("MPI Naive Implementation\n");
        srand(time(NULL)); // Seed for random numbers
        fill_matrix(matrix, matrix_size, vector_size);
        fill_vector(vector, vector_size);
    }

    // Broadcast matrix and vector data to all processes
    MPI_Bcast(vector, vector_size, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < matrix_size; i++) {
        MPI_Bcast(matrix[i], vector_size, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // Calculate rows to process for each MPI process
    int rows_per_process = matrix_size / size;
    int start_row = rank * rows_per_process;
    int end_row = (rank == size - 1) ? matrix_size : start_row + rows_per_process;

    // Each process performs matrix-vector multiplication for its rows
    for (int i = start_row; i < end_row; i++) {
        result[i] = 0;
        for (int j = 0; j < vector_size; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }

    // Gather results from all processes to root process
    if (rank == 0) {
        // Root process receives results from all other processes
        for (int i = 1; i < size; i++) {
            int recv_start_row = i * rows_per_process;
            int recv_end_row = (i == size - 1) ? matrix_size : recv_start_row + rows_per_process;
            MPI_Recv(result + recv_start_row, recv_end_row - recv_start_row, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
    else {
        // Non-root processes send their results to the root process
        MPI_Send(result + start_row, end_row - start_row, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    // Print the result on root process
    if (rank == 0) {
        // Print the matrix, vector, and results
        print_matrix(matrix, matrix_size, vector_size);
        printf("\n");
        print_vector(vector, vector_size);
        printf("\n");
        print_result(result, matrix_size);
    }

    // Free allocated memory
    for (int i = 0; i < matrix_size; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(vector);
    free(result);

    MPI_Finalize();

    return 0;
}
