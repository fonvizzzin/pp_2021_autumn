// Copyright 2021 Moskvin Stanislav
#include <gtest/gtest.h>
#include <time.h>
#include <random>
#include "gauss_jordan.h"
#include <gtest-mpi-listener.hpp>

TEST(parallel_mpi, parallel_calculation_solution_matrix_1) {
    int size_matrix = 3, rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int num_rows_rank = getNumRows(size_matrix, size, rank);
    int* rows = new int[sizeof(int) * num_rows_rank];
    int k = size_matrix + 1;
    double* mtx = new
        double[sizeof(double) * num_rows_rank * (k)];
    double time_start, time_end;
    std::mt19937 gen;
    for (int i = 0; i < num_rows_rank; i++) {
        rows[i] = rank + size * i;
        gen.seed(rows[i] * (k));
        for (int j = 0; j <= size_matrix; j++)
            mtx[i * (k) + j] = gen() % 100 + 1;
    }

    time_start = MPI_Wtime();
    double* res = parallelGaussJordan(size_matrix, num_rows_rank, rows, mtx);
    time_end = MPI_Wtime();
}

TEST(parallel_mpi, parallel_calculation_solution_matrix_2) {
    int size_matrix = 6, rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int num_rows_rank = getNumRows(size_matrix, size, rank);
    int* rows = new int[sizeof(int) * num_rows_rank];
    int k = size_matrix + 1;
    double* mtx = new
        double[sizeof(double) * num_rows_rank * (k)];
    double time_start, time_end;
    std::mt19937 gen;
    for (int i = 0; i < num_rows_rank; i++) {
        rows[i] = rank + size * i;
        gen.seed(rows[i] * (k));
        for (int j = 0; j <= size_matrix; j++)
            mtx[i * (k) + j] = gen() % 100 + 1;
    }
    time_start = MPI_Wtime();
    double* res = parallelGaussJordan(size_matrix, num_rows_rank, rows, mtx);
    time_end = MPI_Wtime();
}

TEST(parallel_mpi, parallel_calculation_solution_matrix_3) {
    int size_matrix = 9, rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int num_rows_rank = getNumRows(size_matrix, size, rank);
    int* rows = new int[sizeof(int) * num_rows_rank];
    int k = size_matrix + 1;
    double* mtx = new
        double[sizeof(double) * num_rows_rank * (k)];
    double time_start, time_end;
    std::mt19937 gen;
    for (int i = 0; i < num_rows_rank; i++) {
        rows[i] = rank + size * i;
        gen.seed(rows[i] * (k));
        for (int j = 0; j <= size_matrix; j++)
            mtx[i * (k) + j] = gen() % 100 + 1;
    }
    time_start = MPI_Wtime();
    double* res = parallelGaussJordan(size_matrix, num_rows_rank, rows, mtx);
    time_end = MPI_Wtime();
}

TEST(parallel_mpi, parallel_calculation_solution_matrix_4) {
    int size_matrix = 12, rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int num_rows_rank = getNumRows(size_matrix, size, rank);
    int* rows = new int[sizeof(int) * num_rows_rank];
    int k = size_matrix + 1;
    double* mtx = new
        double[sizeof(double) * num_rows_rank * (k)];
    double time_start, time_end;
    std::mt19937 gen;
    for (int i = 0; i < num_rows_rank; i++) {
        rows[i] = rank + size * i;
        gen.seed(rows[i] * (k));
        for (int j = 0; j <= size_matrix; j++)
            mtx[i * (k) + j] = gen() % 100 + 1;
    }
    time_start = MPI_Wtime();
    double* res = parallelGaussJordan(size_matrix, num_rows_rank, rows, mtx);
    time_end = MPI_Wtime();
}

TEST(parallel_mpi, parallel_calculation_solution_matrix_5) {
    int size_matrix = 15, rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int num_rows_rank = getNumRows(size_matrix, size, rank);
    int* rows = new int[sizeof(int) * num_rows_rank];
    int k = size_matrix + 1;
    double* mtx = new
        double[sizeof(double) * num_rows_rank * (k)];
    double time_start, time_end;
    std::mt19937 gen;
    for (int i = 0; i < num_rows_rank; i++) {
        rows[i] = rank + size * i;
        gen.seed(rows[i] * (k));
        for (int j = 0; j <= size_matrix; j++)
            mtx[i * (k) + j] = gen() % 100 + 1;
    }
    time_start = MPI_Wtime();
    double* res = parallelGaussJordan(size_matrix, num_rows_rank, rows, mtx);
    time_end = MPI_Wtime();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
