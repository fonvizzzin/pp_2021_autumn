// Copyright 2018 Nesterov Alexander
#include <gtest/gtest.h>
#include <iostream>
#include <iomanip>
#include "./integration_Monte_Carlo_method.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_MPI, Test_sin) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double resPar,resSeq, a = 0., b = 2., c = 0., d = 1.;
    int n = 100000;
    resPar = integralParallel(a, b, c, d, n);
    if (ProcRank == 0) {
        resSeq = integralMonteCarlo(a, b, c, d, n);
        std::cout << std::setprecision(10)<< resPar << std::endl;
        std::cout << std::setprecision(10)<< resSeq << std::endl;
        EXPECT_NEAR(resSeq, resPar, std::numeric_limits<double>::epsilon() * std::max(resPar, resSeq)*16*1000);
    }
}
TEST(Parallel_MPI, Test_tes) {
    EXPECT_NEAR(10, 5, 6);
}

/*
TEST(Parallel_Operations_MPI, Test_Diff) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int global_diff = getParallelOperations(global_vec, count_size_vector, "-");

    if (rank == 0) {
        int reference_diff = getSequentialOperations(global_vec, "-");
        ASSERT_EQ(reference_diff, global_diff);
    }
}

TEST(Parallel_Operations_MPI, Test_Diff_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int global_diff = getParallelOperations(global_vec, count_size_vector, "-");

    if (rank == 0) {
        int reference_diff = getSequentialOperations(global_vec, "-");
        ASSERT_EQ(reference_diff, global_diff);
    }
}

TEST(Parallel_Operations_MPI, Test_Max) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int global_max;
    global_max = getParallelOperations(global_vec, count_size_vector, "max");

    if (rank == 0) {
        int reference_max = getSequentialOperations(global_vec, "max");
        ASSERT_EQ(reference_max, global_max);
    }
}

TEST(Parallel_Operations_MPI, Test_Max_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int global_max;
    global_max = getParallelOperations(global_vec, count_size_vector, "max");

    if (rank == 0) {
        int reference_max = getSequentialOperations(global_vec, "max");
        ASSERT_EQ(reference_max, global_max);
    }
}
*/

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
