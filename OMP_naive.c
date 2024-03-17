/*
 *Programmer(s): Nabeeha, Arwa  and Alisbha
 *Desc: OpenMP version of matrix-vector multiplication.
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

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
    // Perform matrix-vector multiplication using OpenMP
#pragma omp parallel for
    for (int i = 0; i < rows; i++) {
        result[i] = 0;
        for (int j = 0; j < cols; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
         printf("Usage: %s <matrix size> <vector size>\n", argv[0]);
        return 1;
    }
     printf("OMP Naive Implementation\n");
    int matrix_size = atoi(argv[1]);
    int vector_size = atoi(argv[2]);

    // Allocate memory for matrix, vector, and result
    int** matrix = (int**)malloc(matrix_size * sizeof(int*));
    for (int i = 0; i < matrix_size; i++) {
        matrix[i] = (int*)malloc(vector_size * sizeof(int));
    }

    int* vector = (int*)malloc(vector_size * sizeof(int));
    int* result = (int*)malloc(matrix_size * sizeof(int));

    // Fill the matrix and vector with random values
    srand(time(NULL)); // Seed for random numbers
    fill_matrix(matrix, matrix_size, vector_size);
    fill_vector(vector, vector_size);

    // Print the matrix, vector, and results
     print_matrix(matrix, matrix_size, vector_size);
     printf("\n");
     print_vector(vector, vector_size);
     printf("\n");

    // Perform matrix-vector multiplication
    matrix_vector_multiply(matrix, vector, result, matrix_size, vector_size);

    // Print the result
     print_result(result, matrix_size);

    // Free allocated memory
    for (int i = 0; i < matrix_size; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(vector);
    free(result);

    return 0;
}
