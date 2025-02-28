#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void print_matrix(int *matrix, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            printf("%d ", matrix[i * size + j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    int size, num_procs, rank, portion_size;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    size = atoi(argv[1]);
    portion_size = size / num_procs;

    int *matrix1 = NULL;
    int *matrix2 = (int *)malloc(size * size * sizeof(int));
    int *matrix3 = (int *)malloc(size * size * sizeof(int));

    if (rank == 0) {
        matrix1 = (int *)malloc(size * size * sizeof(int));

        srand(time(NULL));
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                matrix1[i * size + j] = rand() % 100 + 1;
                matrix2[i * size + j] = rand() % 100 + 1;
            }
        }
    }

    start_time = MPI_Wtime();

    // Broadcasting matrix2 to all processes manually using MPI_Send and MPI_Recv
    if (rank == 0) {
        for (int i = 1; i < num_procs; ++i) {
            MPI_Send(matrix2, size * size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(matrix2, size * size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Allocating space for sub-matrices
    int *sub_matrix1 = (int *)malloc(portion_size * size * sizeof(int));
    int *sub_matrix3 = (int *)malloc(portion_size * size * sizeof(int));

    // Scattering matrix1 to all processes manually using MPI_Send and MPI_Recv
    if (rank == 0) {
        for (int i = 1; i < num_procs; ++i) {
            MPI_Send(matrix1 + i * portion_size * size, portion_size * size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        memcpy(sub_matrix1, matrix1, portion_size * size * sizeof(int));
    } else {
        MPI_Recv(sub_matrix1, portion_size * size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Performing matrix multiplication
    for (int i = 0; i < portion_size; ++i) {
        for (int j = 0; j < size; ++j) {
            int sum = 0;
            for (int k = 0; k < size; ++k) {
                sum += sub_matrix1[i * size + k] * matrix2[k * size + j];
            }
            sub_matrix3[i * size + j] = sum;
        }
    }

    // Gathering results manually using MPI_Send and MPI_Recv
    if (rank == 0) {
        memcpy(matrix3, sub_matrix3, portion_size * size * sizeof(int));
        for (int i = 1; i < num_procs; ++i) {
            MPI_Recv(matrix3 + i * portion_size * size, portion_size * size, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    } else {
        MPI_Send(sub_matrix3, portion_size * size, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    end_time = MPI_Wtime();

    if (rank == 0) {
        if (size <= 16) {
            printf("Matrix 1:\n");
            print_matrix(matrix1, size);
            printf("Matrix 2:\n");
            print_matrix(matrix2, size);
            printf("Matrix 3:\n");
            print_matrix(matrix3, size);
        }

        printf("Number of processes: %d\t", num_procs);
        printf("Execution time: %.3lf ms\n", (end_time - start_time) * 1000);

        free(matrix1);
    }

    free(matrix2);
    free(matrix3);
    free(sub_matrix1);
    free(sub_matrix3);

    MPI_Finalize();

    return 0;
}
