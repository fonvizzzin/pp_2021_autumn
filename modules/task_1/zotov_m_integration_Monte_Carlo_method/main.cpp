/// Copyright 2018 Nesterov Alexander
#include <gtest/gtest.h>
#include <iostream>
#include <iomanip>
#include "./integration_Monte_Carlo_method.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_MPI, Test_func_1) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double resPar, resSeq;
    double a = 0, b = 2, h = 6;
    int n = 50000;
    resPar = integralParallel(a, b, h, n, f1);
    if (ProcRank == 0) {
        resSeq = integralMonteCarlo(a, b, h, n, f1);
        EXPECT_NEAR(resSeq, resPar, 0.5);
    }
}


TEST(Parallel_MPI, Test_funct_2) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double resPar, resSeq;
    double a = 5, b = 10, h = 6;
    int n = 100000;
    resPar = integralParallel(a, b, h, n, f2);
    if (ProcRank == 0) {
        resSeq = integralMonteCarlo(a, b, h, n, f2);
        EXPECT_NEAR(resSeq, resPar, 0.5);
    }
}

TEST(Parallel_MPI, Test_funct_3) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double resPar, resSeq;
    double a = 0, b = 3, h = 1;
    int n = 100000;
    resPar = integralParallel(a, b, h, n, f3);
    if (ProcRank == 0) {
        resSeq = integralMonteCarlo(a, b, h, n, f3);
        EXPECT_NEAR(resSeq, resPar, 0.5);
    }
}

TEST(Parallel_MPI, Test_funct_4) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double resPar, resSeq;
    double a = 0, b = 3, h = 1;
    int n = 100000;
    resPar = integralParallel(a, b, h, n, f4);
    if (ProcRank == 0) {
        resSeq = integralMonteCarlo(a, b, h, n, f4);
        EXPECT_NEAR(resSeq, resPar, 0.5);
    }
}

TEST(Parallel_MPI, Test_funct_5) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double resPar, resSeq;
    double a = 0, b = 6, h = 10;
    int n = 200000;
    resPar = integralParallel(a, b, h, n, f5);
    if (ProcRank == 0) {
        resSeq = integralMonteCarlo(a, b, h, n, f5);
        EXPECT_NEAR(resSeq, resPar, 0.5);
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

    return 0;
}