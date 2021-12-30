// Copyright 2021 Zotov Maksim
#include <gtest/gtest.h>
#include <vector>
#include "./sparse_matrix_multiplication_ccs.h"
#include <gtest-mpi-listener.hpp>


TEST(Parallel_Operations_MPI, sequential_multiplication_test) {
    Matrix A;
    Matrix B;
    std::vector<double>seq_vec;
    std::vector<double> exp_vec = { 2, 16, 0, 32, 0, 18, 0, 0, 3, 8, 0, 16, 0, 0, 0, 0 };
    int ProcRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    A.value = { 1, 3, 4, 8 };
    A.row = { 0, 1, 1, 3 };
    A.column = { 0, 1, 1, 2, 4 };
    A.size = 4;

    B.value = { 2, 4, 6, 3, 2, 5 };
    B.row = { 0, 3, 2, 0, 3, 1 };
    B.column = { 0, 2, 3, 5, 6 };
    B.size = 4;
    if (ProcRank == 0) {
        seq_vec = multiplication(A, B);
        ASSERT_EQ(exp_vec, seq_vec);
    }
}

TEST(Parallel_Operations_MPI, parallel_multiplication_test) {
    Matrix A;
    Matrix B;
    std::vector<double>par_vec;
    std::vector<double> exp_vec = { 2, 16, 0, 32, 0, 18, 0, 0, 3, 8, 0, 16, 0, 0, 0, 0 };
    int ProcRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    A.value = { 1, 3, 4, 8 };
    A.row = { 0, 1, 1, 3 };
    A.column = { 0, 1, 1, 2, 4 };
    A.size = 4;

    B.value = { 2, 4, 6, 3, 2, 5 };
    B.row = { 0, 3, 2, 0, 3, 1 };
    B.column = { 0, 2, 3, 5, 6 };
    B.size = 4;

    par_vec = parallelMultiplication(A, B);
    if (ProcRank == 0) {
        ASSERT_EQ(exp_vec, par_vec);
    }
}

TEST(Parallel_Operations_MPI, parallel_5_x_5_test) {
    Matrix C;
    Matrix D;
    Matrix G1;
    Matrix G2;
    std::vector<double>par_vec;
    std::vector<double>seq_vec;
    int ProcRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    C = getRandomMatrix(5);
    D = getRandomMatrix(5);
    if (ProcRank == 0) {
        seq_vec = multiplication(C, D);
    }
    par_vec = parallelMultiplication(C, D);
    if (ProcRank == 0) {
        ASSERT_EQ(seq_vec, par_vec);
    }
}

TEST(Parallel_Operations_MPI, parallel_10_x_10_test) {
    Matrix C;
    Matrix D;
    Matrix G1;
    Matrix G2;
    std::vector<double>par_vec;
    std::vector<double>seq_vec;
    int ProcRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    C = getRandomMatrix(10);
    D = getRandomMatrix(10);
    if (ProcRank == 0) {
        seq_vec = multiplication(C, D);
    }
    par_vec = parallelMultiplication(C, D);

    if (ProcRank == 0) {
        ASSERT_EQ(seq_vec, par_vec);
    }
}

TEST(Parallel_Operations_MPI, parallel_100_x_100_test) {
    Matrix C;
    Matrix D;
    Matrix G1;
    Matrix G2;
    std::vector<double>par_vec;
    std::vector<double>seq_vec;
    int ProcRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    C = getRandomMatrix(100);
    D = getRandomMatrix(100);
    if (ProcRank == 0) {
        seq_vec = multiplication(C, D);
    }
    par_vec = parallelMultiplication(C, D);

    if (ProcRank == 0) {
        ASSERT_EQ(seq_vec, par_vec);
    }
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
